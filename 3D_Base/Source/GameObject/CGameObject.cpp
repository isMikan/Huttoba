#include "CGameObject.h"

//コンストラクタ.
CGameObject::CGameObject()
	: m_vPosition	()
	, m_vRotation	()
	, m_vScale		( 1.0f, 1.0f, 1.0f )

	, m_vQuaternion	( D3DXQUATERNION(0.f, 0.f, 0.f, 1.f) )

	, m_IsWorldMatrixDirty	{ true }
	, m_mWorldCache			{}
{
}

//デストラクタ.
CGameObject::~CGameObject()
{
}

const D3DXMATRIX& CGameObject::GetWorldMatrix() const
{
	if (m_IsWorldMatrixDirty)
	{
		//ワールド行列、スケール行列、回転行列、平行移動行列.
		D3DXMATRIX mWorld, mScale, mRotQuat, mTran;

		//拡大縮小行列作成.
		D3DXMatrixScaling(
			&mScale,	//(out)計算結果.
			m_vScale.x, m_vScale.y, m_vScale.z);	//x,y,zそれぞれの拡縮値.
		//平行移動行列作成.
		D3DXMatrixTranslation(
			&mTran,	//(out)計算結果.
			m_vPosition.x, m_vPosition.y, m_vPosition.z);	//x,y,z座標.

		//クォータニオンから回転行列を作成.
		D3DXMatrixRotationQuaternion(&mRotQuat, &m_vQuaternion);
		//ワールド行列作成.
		//拡縮×回転×移動 ※順番がとても大切！！.
		m_mWorldCache = mScale * mRotQuat * mTran;

		m_IsWorldMatrixDirty = false;
	}
	return m_mWorldCache;
}
