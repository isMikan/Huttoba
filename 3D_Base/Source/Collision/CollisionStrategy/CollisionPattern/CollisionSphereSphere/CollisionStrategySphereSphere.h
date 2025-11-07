#pragma once

#include"Collision/CollisionStrategy/CollisionStrategyBase.h"

class CollisionBase;

/***************************************************************************************
 * 衝突判定ロジッククラス：球体 vs. 球体 (Strategy)
 * 役割: 2つの CollisionSphere の情報を使用して、衝突しているかを計算する。
 * このクラスは状態を持たず、静的メソッドとして利用
 *********/

class CollisionStrategySphereSphere : public CollisionStrategyBase
{

public:

    /*****************************************************************************************
    * @brief    2つの球体が衝突しているかどうかを判定
    * 
    * @param    sphereA 1つ目の球体情報 (半径とローカルオフセットの取得元)
    *           sphereB 2つ目の球体情報
    * 
    * @return   true  : 衝突
    *           false : 衝突していない 
    *****************************************************************************************/
    virtual bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};