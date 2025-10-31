#include "stdafx.h"
#include "MeshCollisionUtility.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"

namespace MeshCollisionUtility
{
    bool CalculateBoundingSphere(
        const std::shared_ptr<CStaticMesh> pMesh,
        D3DXVECTOR3& outCenter,
        float& outRadius)
    {
        if (!pMesh || !pMesh->GetMesh())
        {
            return false;
        }

        LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ
        void* pVertices = nullptr;				//頂点

        //頂点バッファを取得
        if (FAILED(pMesh->GetMesh()->GetVertexBuffer(&pVB)))
        {
            return false;
        }

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
        if (pVB != nullptr) {
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
        if (!pMeshDx9)
        {
            return false;
        }

        // 頂点データのロックと取得
        VOID* pVertices = nullptr;
        if (FAILED(pMeshDx9->LockVertexBuffer(0, &pVertices)))
        {
            return false;
        }

        // 頂点の情報（VERTEX構造体の定義はCStaticMesh.hから持ってくる）
        // VERTEX構造体は Pos, Normal, UV の順で、Posは先頭にあることを確認済み。
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
}