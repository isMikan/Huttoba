#pragma once
#include "Collision/Collider/CollisionBase.h"

class CollisionSphere : public CollisionBase
{
public:
    CollisionSphere();
    ~CollisionSphere();

    // ワールド行列更新
    void UpdateWorldMat() override;

    // ワールド行列取得
    const D3DXMATRIX& GetWorldMat() const override { return m_World; }

    // ワールド空間上の中心点取得
    D3DXVECTOR3 GetWorldPos() const override { return m_WorldCenter; }

    // ColliderType
    ColliderType GetType() override { return ColliderType::Sphere; }

    // 半径設定・取得
    void SetRadius(float r) { m_Radius = r; }
    float GetRadius() const { return m_Radius; }

    void DrawDebug();

    // ローカル中心設定
    void SetCenter(const D3DXVECTOR3& center) { m_LocalCenter = center; }

    // ローカル中心取得
    D3DXVECTOR3 GetCenter() const { return m_LocalCenter; }

private:
    D3DXMATRIX m_World;       // ワールド行列
    D3DXVECTOR3 m_LocalCenter; // ローカル座標での中心
    D3DXVECTOR3 m_WorldCenter; // ワールド座標での中心
    float m_Radius;            // 球の半径
};
