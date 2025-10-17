#pragma once
#include "Collision/Collider/CollisionBase.h"
class CollisionRay
	: public CollisionBase
{
public:
    CollisionRay();
    ~CollisionRay();

    //ローカル座標をワールドに変換
    void UpdateWorldMat() override;

    //ワールド行列を取得
    const D3DXMATRIX& GetWorldMat() const override { return m_World; }

    //現在座標の始点を取得
    D3DXVECTOR3 GetWorldPos() const override { return D3DXVECTOR3(m_World._41, m_World._42, m_World._43); }


    // 設定関数
    void SetOrigin(const D3DXVECTOR3& origin) { m_WorldOrigin = origin; }
    void SetDirection(const D3DXVECTOR3& dir) { m_Direction = dir; NormalizeDirection(); }
    void SetLength(float length) { m_Length = length; }

    // 取得関数
    const D3DXVECTOR3& GetOrigin() const { return m_WorldOrigin; }
    const D3DXVECTOR3& GetDirection() const { return m_Direction; }
    float GetLength() const { return m_Length; }

    // ワールド空間上の終点を取得
    D3DXVECTOR3 GetEndPoint() const;

private:
    void NormalizeDirection();

private:

    D3DXVECTOR3 m_LocalOrigin;   // モデル内での始点
    D3DXVECTOR3 m_WorldOrigin;   // ワールド空間での始点

    D3DXVECTOR3 m_Direction;   // 方向ベクトル（正規化済み）
    float m_Length;            // レイの長さ
    D3DXMATRIX m_World;        // ワールド行列
};