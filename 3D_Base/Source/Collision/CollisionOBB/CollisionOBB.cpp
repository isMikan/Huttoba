#include "stdafx.h"
#include "CollisionOBB.h"

CollisionObb::CollisionObb()
	: m_Size		{1.f,1.f,1.f}
	, m_LocalAxes	{}
	, m_World		{}
{
	// 初期化
	D3DXMatrixIdentity(&m_World);
	m_LocalAxes[0] = D3DXVECTOR3(1, 0, 0);
	m_LocalAxes[1] = D3DXVECTOR3(0, 1, 0);
	m_LocalAxes[2] = D3DXVECTOR3(0, 0, 1);
}

CollisionObb::~CollisionObb()
{
}

void CollisionObb::UpdateWorldMat()
{
	//スケール・回転・移動の行列作成
	D3DXMATRIX matS, matR, matT;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	//ワールド行列を計算
	m_World = matS * matR * matT;

	//ローカル軸を更新
	m_LocalAxes[0] = D3DXVECTOR3(m_World._11, m_World._21, m_World._31); // X軸
	m_LocalAxes[1] = D3DXVECTOR3(m_World._12, m_World._22, m_World._32); // Y軸
	m_LocalAxes[2] = D3DXVECTOR3(m_World._13, m_World._23, m_World._33); // Z軸

	// 正規化（数値誤差で軸が伸びるのを防ぐ）
	for (int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&m_LocalAxes[i], &m_LocalAxes[i]);
}