#pragma once

class CStaticMesh;

/***************************************
 * メッシュの頂点情報から衝突パラメータを計算するユーティリティ。
 *******/
namespace MeshCollisionUtility
{
    /*****************************************************************************************
    * @brief    メッシュの外接球の中心座標と半径を計算します
    * @param    pMesh 頂点情報を持つCStaticMeshの共有ポインタ
    *           outCenter 計算された中心座標の参照
    *           outRadius 計算された半径の参照
    * 
    * @return   計算が成功したかどうか
    *****************************************************************************************/
    bool CalculateBoundingSphere(
        const std::shared_ptr<CStaticMesh> pMesh,
        D3DXVECTOR3& outCenter,
        float& outRadius
    );
}