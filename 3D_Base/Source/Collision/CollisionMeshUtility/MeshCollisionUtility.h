#pragma once

class CStaticMesh;

/**
 * メッシュの頂点情報から衝突パラメータを計算するユーティリティ。
 */
namespace MeshCollisionUtility
{
    /**
     * メッシュの外接球の中心座標と半径を計算します。
     * @param pMesh 頂点情報を持つCStaticMeshの共有ポインタ。
     * @param outCenter 計算された中心座標の参照。
     * @param outRadius 計算された半径の参照。
     * @return 計算が成功したかどうか (true/false)。
     */
    bool CalculateBoundingSphere(
        const std::shared_ptr<CStaticMesh> pMesh,
        D3DXVECTOR3& outCenter,
        float& outRadius
    );
}