#pragma once

#include "Collision/Collider/CollisionBase.h" 

/**
 * 球体の衝突情報クラス。
 * 役割: CollisionBase の共通情報に加え、球体固有のデータ（半径）を保持する。
 */
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
    // ----------------------------------------------------
    // 球体固有のデータアクセス
    // ----------------------------------------------------

    /**
     * 球体の半径を取得します。
     * @return 半径の値
     */
    float GetRadius() const { return m_Radius; }

    /**
     * 球体の半径を設定します。
     * @param radius 設定する半径
     */
    void SetRadius(float radius) {
        // 半径が負にならないよう簡単なチェックを行う
        m_Radius = (radius > 0.0f) ? radius : 0.0f;
    }

    // ----------------------------------------------------
    // 基底クラスの純粋仮想関数の実装
    // ----------------------------------------------------

    /**
     * この衝突形状のタイプを返します。（必須実装）
     * @return ColliderType::Sphere
     */
    virtual ColliderType GetType() const override {
        return ColliderType::Sphere;
    }


private:
    // 球体固有のデータ
    float m_Radius;

};