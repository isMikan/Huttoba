#pragma once
#include "Collision/CollisionBase.h"
class CollisionObb
	: public CollisionBase
{
public:
	CollisionObb();
	~CollisionObb();

	void UpdateWorldMat() override;
	const D3DXMATRIX& GetWorldMat() const override { return m_World; }
	D3DXVECTOR3 GetWorldPos() const override { return D3DXVECTOR3(m_World._41, m_World._42, m_World._43); }

	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	const D3DXVECTOR3& GetSize() const { return m_Size; }

private:

	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_LocalAxes[3];
	D3DXMATRIX m_World;
};

