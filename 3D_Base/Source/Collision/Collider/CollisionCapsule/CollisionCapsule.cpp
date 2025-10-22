//#include "stdafx.h"
//#include "CollisionCapsule.h"
//
//CollisionCapsule::CollisionCapsule()
//	: m_Start(0.0f, 0.0f, 0.0f)
//	, m_End(0.0f, 1.0f, 0.0f)
//	, m_Radius(0.5f)
//	, m_StartWorld	{}
//	, m_EndWorld	{}
//{
//	D3DXMatrixIdentity(&m_World);
//}
//
//CollisionCapsule::~CollisionCapsule()
//{
//}
//
//void CollisionCapsule::UpdateWorldMat()
//{
//	// スケール・回転・平行移動行列の作成
//	D3DXMATRIX matS, matR, matT;
//	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
//	D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
//	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//
//	m_World = matS * matR * matT;
//
//	// ワールド変換済み位置を保存
//	D3DXVec3TransformCoord(&m_StartWorld, &m_Start, &m_World);
//	D3DXVec3TransformCoord(&m_EndWorld, &m_End, &m_World);
//}
//
//float CollisionCapsule::GetWorldRadius() const
//{
//	// スケールの平均値を半径に適用（概算）
//	float scale = (m_vScale.x + m_vScale.y + m_vScale.z) / 3.0f;
//	return m_Radius * scale;
//}
