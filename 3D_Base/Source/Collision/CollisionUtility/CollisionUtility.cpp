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

        // --- 1. カプセル半径（太さ）の計算 ---
        // Y軸を軸とするため、半径はX軸とZ軸の幅の大きい方で決まる
        float halfWidthX = (maxX - minX) * 0.5f;
        float halfWidthZ = (maxZ - minZ) * 0.5f;

        // カプセル半径
        outRadius = std::max(halfWidthX, halfWidthZ);

        // --- 2. 軸線分 A, B の計算（Y軸） ---
        float totalWidthY = maxY - minY;

        // 軸線分の長さ (Y軸の全長から両端の直径 (2R) を引く)
        float coreLength = totalWidthY - (2.0f * outRadius);

        // Y軸の中点（カプセル全体の中心）
        float centerY = (minY + maxY) * 0.5f;

        // XZ平面の中心点（カプセル軸の位置）
        float centerX = (minX + maxX) * 0.5f;
        float centerZ = (minZ + maxZ) * 0.5f;

        // 軸線分が潰れる場合の処理 (球体判定)
        if (coreLength <= 0.0f)
        {
            // 軸線分を中点に集約
            outLocalOffsetA = D3DXVECTOR3(centerX, centerY, centerZ);
            outLocalOffsetB = D3DXVECTOR3(centerX, centerY, centerZ);
        }
        else
        {
            float halfLength = coreLength * 0.5f;

            // Y軸に沿ってオフセットを設定
            // A: 上端側, B: 下端側
            outLocalOffsetA = D3DXVECTOR3(centerX, centerY + halfLength, centerZ);
            outLocalOffsetB = D3DXVECTOR3(centerX, centerY - halfLength, centerZ);
        }

        return true;
    }

    bool CalculateHorizontalCapsule(
        const std::shared_ptr<CStaticMesh>& pMesh, 
        float& outRadius,
        D3DXVECTOR3& outOffsetA, 
D3DXVECTOR3& outOffsetB)
    {
        if (!pMesh) return false;

        // 各軸の最小/最大値を見つける
        D3DXVECTOR3 min = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
        D3DXVECTOR3 max = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        DWORD stride = 0;
        UINT count = 0;

        //  Lockを行い、頂点データへのポインタを取得
        LPVOID pData = pMesh->GetLockedVertexBuffer(stride, count);
        if (!pData || count == 0) return false;

        // 頂点データはVERTEX構造体の配列として扱う
        const BYTE* pVertices = static_cast<const BYTE*>(pData);

        // 頂点データを走査
        for (UINT i = 0; i < count; ++i)
        {
            // Xファイルの頂点レイアウトでは、POSITIONはオフセット0にあると仮定
            const D3DXVECTOR3& pos = *reinterpret_cast<const D3DXVECTOR3*>(pVertices + i * stride);

            min.x = std::min(min.x, pos.x);
            min.y = std::min(min.y, pos.y);
            min.z = std::min(min.z, pos.z);

            max.x = std::max(max.x, pos.x);
            max.y = std::max(max.y, pos.y);
            max.z = std::max(max.z, pos.z);
        }

        pMesh->UnlockVertexBuffer();
        // X軸に伸びるから、半径はY軸とZ軸の最大幅の半分になる。
        float halfHeight = (max.y - min.y) * 0.5f;
        float halfDepth = (max.z - min.z) * 0.5f;

        // YとZの幅の大きい方をカプセルの厚みとして採用（真円にするため）
        outRadius = std::max(halfHeight, halfDepth);

        // 全長の中央
        float centerX = (min.x + max.x) * 0.5f;
        float centerY = (min.y + max.y) * 0.5f;
        float centerZ = (min.z + max.z) * 0.5f;

        // X軸の端点位置
        float endX_A = max.x - outRadius;
        float endX_B = min.x + outRadius;

        float totalWidthX = max.x - min.x;
        float axisLength = totalWidthX - (2.0f * outRadius);

        // メッシュが短すぎる場合、球にする
        if (axisLength < 0.0f) {
            endX_A = endX_B = centerX;
        }
        else {
            // 軸の半分の長さを計算
            float halfAxisLength = axisLength * 0.5f;

            // 中心から軸の長さに従ってオフセット
            endX_A = centerX + halfAxisLength;
            endX_B = centerX - halfAxisLength;
        }
        // Y/Z座標は修正案1を適用
        outOffsetA = D3DXVECTOR3(endX_A, centerY, centerZ);
        outOffsetB = D3DXVECTOR3(endX_B, centerY, centerZ);

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