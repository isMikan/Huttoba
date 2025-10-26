#pragma once


/*************************************
 * 当たり判定を行うオブジェクトが実装するインターフェース
 * 
 * CollisionManagerでこのメソッドを呼んで処理する
 *****/

class CollisionBase;

class ICollisionListener
{
public:
    virtual ~ICollisionListener() {}

    /*****************************************************************************************
    * @brief    衝突判定したときに呼び出される関数
    * @param    other : 当たり判定の主(衝突処理の関数で渡されるからそれを入れるだけ)
    *****************************************************************************************/
    virtual void OnCollision(CollisionBase* other) = 0;
};