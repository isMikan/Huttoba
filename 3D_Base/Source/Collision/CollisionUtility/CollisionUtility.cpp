#include "stdafx.h"
#include "CollisionUtility.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "Ground/GroundManager/CGroundManager.h"

namespace CollisionUtility
{
    bool CalculateBoundingSphere(
        const std::shared_ptr<CStaticMesh> pMesh,
        D3DXVECTOR3& outCenter,
        float& outRadius)
    {
        if (!pMesh || !pMesh->GetMesh()) { return false; }

        LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ
        void* pVertices = nullptr;				//頂点

        //頂点バッファを取得
        if (FAILED(pMesh->GetMesh()->GetVertexBuffer(&pVB))) { return false; }

        //メッシュの頂点バッファをロックする
        if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
        {
            SAFE_RELEASE(pVB);
            return false;
        }

        //メッシュの外接円の中心と半径を計算する
        D3DXComputeBoundingSphere(
            static_cast<D3DXVECTOR3*>(pVertices),
            pMesh->GetMesh()->GetNumVertices(),					//頂点の数
            D3DXGetFVFVertexSize(pMesh->GetMesh()->GetFVF()),	//頂点の情報
            &outCenter,											//(out)中心座標
            &outRadius);										//(out)半径

        //メッシュの頂点バッファをアンロックする
        if (pVB != nullptr)
        {
            pVB->Unlock();
            SAFE_RELEASE(pVB); // 取得したポインタを解放
        }

        return true;
    }


    bool CalculateBoundingCapsule(
        const std::shared_ptr<CStaticMesh> pMesh,
        float& outRadius,
        D3DXVECTOR3& outLocalOffsetA,
        D3DXVECTOR3& outLocalOffsetB)
    {
        LPD3DXMESH pMeshDx9 = pMesh->GetMesh();
        if (!pMeshDx9) { return false; }

        // 頂点データのロックと取得
        VOID* pVertices = nullptr;
        if (FAILED(pMeshDx9->LockVertexBuffer(0, &pVertices))) { return false; }

        // 頂点の情報（VERTEX構造体の定義はCStaticMesh::VERTEXを使用）
        using VERTEX = CStaticMesh::VERTEX;
        VERTEX* vertices = static_cast<VERTEX*>(pVertices);
        DWORD numVertices = pMeshDx9->GetNumVertices();

        if (numVertices == 0)
        {
            pMeshDx9->UnlockVertexBuffer();
            return false;
        }

        // Min/Maxの初期値設定（最初の頂点で初期化）
        float minX = vertices[0].Pos.x, maxX = vertices[0].Pos.x;
        float minY = vertices[0].Pos.y, maxY = vertices[0].Pos.y;
        float minZ = vertices[0].Pos.z, maxZ = vertices[0].Pos.z;

        // 全頂点を走査し、各軸のMin/Maxを求める
        for (DWORD i = 0; i < numVertices; ++i)
        {
            const D3DXVECTOR3& pos = vertices[i].Pos;

            minX = std::min(minX, pos.x); maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y); maxY = std::max(maxY, pos.y);
            minZ = std::min(minZ, pos.z); maxZ = std::max(maxZ, pos.z);
        }
        pMeshDx9->UnlockVertexBuffer();

        // カプセル半径の計算
        float halfWidthX = (maxX - minX) * 0.5f;
        float halfWidthZ = (maxZ - minZ) * 0.5f;
        outRadius = std::max(halfWidthX, halfWidthZ);

        //軸線分 A, B の計算

        // Y軸の全長
        float totalWidthY = maxY - minY;

        // 軸線分の長さ (Y軸の全長から両端の直径 (2R) を引く)
        float coreLength = totalWidthY - (2.0f * outRadius);

        // 【重要】メッシュの実際の中心座標は、オフセット計算に含めない
        // float centerY = (minY + maxY) * 0.5f; // 不要

        // 軸線分が潰れる場合の処理 (球体判定)
        if (coreLength <= 0.0f)
        {
            // 軸線分をローカル原点に集約
            outLocalOffsetA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            outLocalOffsetB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        }
        else
        {
            float halfLength = coreLength * 0.5f;

            // XZ座標は常に 0.0f に固定する
            // A: 上端側 (+Y軸方向)
            outLocalOffsetA = D3DXVECTOR3(0.0f, halfLength, 0.0f);
            // B: 下端側 (-Y軸方向)
            outLocalOffsetB = D3DXVECTOR3(0.0f, -halfLength, 0.0f);
        }

        return true;
    }

    bool RaycastAgainstMesh(
        const std::shared_ptr<CStaticMesh> pTargetMesh,
        const D3DXVECTOR3& rayOrigin,
        const D3DXVECTOR3& rayDirection,
        float maxDistance,
        D3DXVECTOR3& outHitPos
    )
    {
        //ステージがz軸に10ずれているから、ワールド行列を取得して逆行列計算
        D3DXMATRIX matWorld = pTargetMesh->GetWorldMatrix();
        D3DXMATRIX matInverseWorld;
        D3DXMatrixInverse(&matInverseWorld, nullptr, &matWorld);

        //レイをメッシュのローカル座標に
        D3DXVECTOR3 localRayOrigin;
        D3DXVECTOR3 localRayDirection;

        // 始点 (座標) を変換
        D3DXVec3TransformCoord(&localRayOrigin, &rayOrigin, &matInverseWorld);
        // 方向 (ベクトル) を変換し、正規化
        D3DXVec3TransformNormal(&localRayDirection, &rayDirection, &matInverseWorld);
        D3DXVec3Normalize(&localRayDirection, &localRayDirection);

        BOOL bHit = FALSE;
        FLOAT fDist = 0.0f;
        FLOAT U = 0, V = 0;		//重心ヒット座標

        // レイとメッシュの交差判定
        HRESULT hr = D3DXIntersect(
            pTargetMesh->GetMeshForRay(),   // ターゲットメッシュ
            &localRayOrigin,                // レイの始点
            &localRayDirection,             // レイの方向
            &bHit,                          // 衝突したか
            nullptr,                        // 衝突した面のインデックス
            &fDist,                         // 距離
            nullptr,                        // Barycentric U
            nullptr,                        // Barycentric V
            nullptr,                        // すべてのヒット情報
            nullptr                         // 衝突グループID
        );

        if (FAILED(hr) || !bHit)
        {
            return false;
        }

        // 距離の検証
        if (fDist > maxDistance)
        {
            return false;
        }

        //ローカルの位置を特定し、ワールド行列に変換
        D3DXVECTOR3 localHitPos = localRayOrigin + (localRayDirection * fDist);
        D3DXVec3TransformCoord(&outHitPos, &localHitPos, &matWorld);

        return true;
    }

    bool CheckGroundContact(
        const D3DXVECTOR3& objectPosition,
        CGroundManager& pGroundMgr,
        float& outGroundY)
    {
        // レイ設定 
        D3DXVECTOR3 rayOrigin = objectPosition + D3DXVECTOR3(0.0f, 0.3f, 0.0f);
        D3DXVECTOR3 rayDirection(0.0f, -1.0f, 0.0f);
        const float maxDistance = 5.f;

        // Raycastのout引数
        D3DXVECTOR3 hitPosition;
        bool hitGround = false;

        // 地面の数だけ回す
        for (const auto& pGround : pGroundMgr.GetGrounds())
        {
            if (!pGround || pGround->GetIsFallDown()) continue;

            // メッシュを取得
            std::shared_ptr<CStaticMesh> pGroundMesh = pGround->GetMesh();
            if (!pGroundMesh) continue;

            // MeshCollisionUtilityに判定を委譲
            if (CollisionUtility::RaycastAgainstMesh(
                pGroundMesh,    // 地面のメッシュ
                rayOrigin,      // レイの始点
                rayDirection,   // レイの向き
                maxDistance,    // レイの最大距離
                hitPosition     // out 当たった場所
            ))
            {
                hitGround = true;
                outGroundY = hitPosition.y;
                break;
            }
        }

        return hitGround;
    }

}