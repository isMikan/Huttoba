#pragma once

#include "Collision/Collider/CollisionBase.h" 

/***************************************
 * 球体の衝突情報クラス。
 * 役割: CollisionBase の共通情報に加え、球体固有のデータ（半径）を保持する。
 **********/
class CollisionSphere : public CollisionBase
{

public:
    // コンストラクタ
    CollisionSphere(
        ICollisionListener* pListener,
        const D3DXVECTOR3& syncPosition,
        ColliderTag tag,
        float radius,
        const D3DXVECTOR3& localOffset
    );

    /*****************************************************************************************
    * @brief    球の半径を取得
    * 
    * @return   球の半径   
    *****************************************************************************************/
    float GetRadius() const { return m_Radius; }


    /*****************************************************************************************
    * @brief    球の半径を設定
    *****************************************************************************************/
    void SetRadius(float radius) 
    {
        // 半径が負にならないように
        m_Radius = (radius > 0.0f) ? radius : 0.0f;
    }

    /*****************************************************************************************
    * @brief    当たり判定の形を取得
    * 
    * @return   当たり判定識別のタグを返す   
    *****************************************************************************************/

    virtual ColliderType GetType() const override {
        return ColliderType::Sphere;
    }


private:
    // 球体固有のデータ
    float m_Radius;

};