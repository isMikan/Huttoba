#pragma once
#include "Collision/Collider/CollisionBase.h"
class CollisionObb
	: public CollisionBase
{
public:
	CollisionObb();
	~CollisionObb();

	//ローカル座標をワールドに変換
	void UpdateWorldMat() override;

	//ワールド行列を取得
	const D3DXMATRIX& GetWorldMat() const override { return m_World; }

	//現在座標の中心を取得
	D3DXVECTOR3 GetWorldPos() const override { return D3DXVECTOR3(m_World._41, m_World._42, m_World._43); }



	void SetSize(D3DXVECTOR3 size) { m_Size = size; }

	const D3DXVECTOR3& GetSize() const { return m_Size; }
	D3DXVECTOR3 GetHalfSize() const { return m_Size * 0.5f; }
	const D3DXVECTOR3& GetAxis(int index) const { return m_LocalAxes[index]; }
	ColliderType GetType() { return ColliderType::OBB; }

private:

	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_LocalAxes[3];
	D3DXMATRIX m_World;
};

