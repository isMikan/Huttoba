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

        // 頂点の情報
        using VERTEX = CStaticMesh::VERTEX;
        VERTEX* vertices = static_cast<VERTEX*>(pVertices);
        DWORD numVertices = pMeshDx9->GetNumVertices();

        if (numVertices == 0)
        {
            pMeshDx9->UnlockVertexBuffer();
            return false;
        }

        // Min/Maxと最大半径の初期値設定
        float minY = vertices[0].Pos.y;
        float maxY = vertices[0].Pos.y;
        float maxRadiusSq = 0.0f;

        // 全頂点を走査し、Y軸のMin/MaxとXZ平面の最大半径を求める
        for (DWORD i = 0; i < numVertices; ++i)
        {
            const D3DXVECTOR3& pos = vertices[i].Pos;

            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);

            // XZ平面での原点からの距離の2乗を計算 (XZ平面の最大距離が半径になる)
            float currentRadiusSq = pos.x * pos.x + pos.z * pos.z;
            maxRadiusSq = std::max(maxRadiusSq, currentRadiusSq);
        }

        // 頂点バッファのアンロック
        pMeshDx9->UnlockVertexBuffer();

        // カプセルのパラメータを設定
        outRadius = sqrtf(maxRadiusSq);

        // カプセルの軸線分を決定する (メッシュの端から半径分だけ内側に入れる)
        // A: 下端 (MinY) + 半径R
        outLocalOffsetA = D3DXVECTOR3(0.0f, minY + outRadius, 0.0f);
        // B: 上端 (MaxY) - 半径R
        outLocalOffsetB = D3DXVECTOR3(0.0f, maxY - outRadius, 0.0f);

        // 軸線分AがBより上に来る場合（平たい、または球体）
        if (outLocalOffsetA.y >= outLocalOffsetB.y)
        {
            // 軸線分を中点に集約し、実質的な球体として扱う
            float center_y = (minY + maxY) / 2.0f;
            outLocalOffsetA = D3DXVECTOR3(0.0f, center_y, 0.0f);
            outLocalOffsetB = D3DXVECTOR3(0.0f, center_y, 0.0f);
        }

        return true;
    }

    bool CalculateHorizontalCapsule(const std::shared_ptr<CStaticMesh>& pMesh, float& outRadius, D3DXVECTOR3& outOffsetA, D3DXVECTOR3& outOffsetB)
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

        // 頂点データはVERTEX構造体の配列として扱えます
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

        // 全長の中央 (X軸中心)
        float centerX = (min.x + max.x) * 0.5f;

        // X軸の端点位置
        float endX_A = max.x - outRadius;
        float endX_B = min.x + outRadius;

        // 軸線分が内側に入りすぎないかチェック 
        if (endX_A < endX_B) {
            // メッシュが短すぎる場合は、軸線分を潰して中心点にする（球になる）
            endX_A = endX_B = centerX;
        }

        // YとZは中心 (0) に固定
        outOffsetA = D3DXVECTOR3(endX_A, 0.0f, 0.0f);
        outOffsetB = D3DXVECTOR3(endX_B, 0.0f, 0.0f);

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
        CGroundManager* pGroundMgr,
        float& outGroundY)
    {
        if (!pGroundMgr) return false;

        // レイ設定 
        D3DXVECTOR3 rayOrigin = objectPosition + D3DXVECTOR3(0.0f, 0.3f, 0.0f);
        D3DXVECTOR3 rayDirection(0.0f, -1.0f, 0.0f);
        const float maxDistance = 5.f;

        // Raycastのout引数
        D3DXVECTOR3 hitPosition;
        bool hitGround = false;

        // 地面の数だけ回す
        for (const auto& pGround : pGroundMgr->GetGrounds())
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