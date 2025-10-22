#pragma once

#include"Collision/CollisionStrategy/CollisionStrategyBase.h"

class CGameObject;
class CollisionSphere;

/**
 * 衝突判定ロジッククラス：球体 vs. 球体 (Strategy)
 * 役割: 2つの CollisionSphere の情報を使用して、衝突しているかを計算する。
 * このクラスは状態を持たず、静的メソッドとして純粋なロジックを提供します。
 */

class CollisionStrategySphereSphere : public CollisionStrategyBase
{

public:

    /*****************************************************************************************
    * @brief    2つの球体が衝突しているかどうかを判定
    * 
    * @param    objA    球Aを所有するゲームオブジェクト (ワールド絶対座標の取得元)
    *           sphereA 1つ目の球体情報 (半径とローカルオフセットの取得元)
    *           objB    球Bを所有するゲームオブジェクト
    *           sphereB 2つ目の球体情報
    * 
    * @return   true  : 衝突
    *           false : 衝突していない 
    *****************************************************************************************/
    static bool CheckCollision(
        const CGameObject* objA,
        const CollisionSphere* sphereA,
        const CGameObject* objB,
        const CollisionSphere* sphereB);

private:

    /*****************************************************************************************
    * @brief    2点間の距離の2乗を計算する。
    * 
    * @param    p1 1つ目の点
    *           p2 2つ目の点
    * 
    * @return   2点間の距離の2乗
    * 
    * @note     平方根は使用していない。
    *           当たり判定は多くのタイミングで呼び出されることが想定されるので
    *           処理負荷がすごいsqrt()は使用しない
    *****************************************************************************************/
    static float GetDistanceSq(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2);
};