#pragma once

/*--------------------

・円判定作成         : CalculateBoundingSphere
・カプセル判定作成   : CalculateBoundingCapsule
・横カプセル判定作成 : CalculateHorizontalCapsule

----------------------

・レイの衝突判定        : RaycastAgainstMesh
・地面に乗っているか判定: CheckGroundContact

*/

class CStaticMesh;

namespace CollisionUtility
{
    /*****************************************************************************************
    * @brief    メッシュの外接球の中心座標と半径を計算
    * 
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


    /*****************************************************************************************
    * @brief    メッシュのカプセルのの中心座標と半径を計算
    * 
    * @param    pMesh 頂点情報を持つCStaticMeshの共有ポインタ
    *           outCenter 計算された中心座標の参照
    *           outRadius 計算された半径の参照
    *
    * @return   計算が成功したかどうか
    *****************************************************************************************/
    bool CalculateBoundingCapsule(
        const std::shared_ptr<CStaticMesh> pMesh,
        float& outRadius,
        D3DXVECTOR3& outLocalOffsetA,
        D3DXVECTOR3& outLocalOffsetB
    );

    /*****************************************************************************************
    * @brief   メッシュから横軸(X)に沿ったカプセルのパラメータを計算
    * 
    * @param   pMesh: 入力メッシュ
    * @param   outRadius: 計算された半径 (Y, Z軸方向の最大幅の半分)
    * @param   outOffsetA: 軸線分A (X軸+側)
    * @param   outOffsetB: 軸線分B (X軸-側)
    * 
    * @return  成功したか
    *****************************************************************************************/
    bool CalculateHorizontalCapsule(
        const std::shared_ptr<CStaticMesh>& pMesh,
        float& outRadius,
        D3DXVECTOR3& outOffsetA,
        D3DXVECTOR3& outOffsetB
    );
    /*****************************************************************************************
    * @brief   レイとターゲットメッシュとの交点を計算
    *
    * @param   pTargetMesh : 判定対象のメッシュオブジェクト
    *          rayOrigin   : レイの始点（ワールド座標）
    *          rayDirection: レイの方向（単位ベクトル）
    *          maxDistance : 最大距離
    *          outHitPos   : (out) 交点座標
    *          outDistance : (out) 始点から交点までの距離
    *
    * @return  交点が見つかったかどうか
    *****************************************************************************************/
    bool RaycastAgainstMesh(
        const std::shared_ptr<CStaticMesh> pTargetMesh,
        const D3DXVECTOR3& rayOrigin,
        const D3DXVECTOR3& rayDirection,
        float maxDistance,
        D3DXVECTOR3& outHitPos
    );

    /*****************************************************************************************
    * @brief   オブジェクトの地面との接触を判定する
    * 
    * @param    objectPosition     : 計算したいオブジェクトの座標
    *           colliderHalfHeight : 当たり判定の半分のサイズ
    *           pGroundMgr         :
    *****************************************************************************************/
    bool CheckGroundContact(
        const D3DXVECTOR3& objectPosition,
        CGroundManager& pGroundMgr,
        float& outGroundY
    );

}