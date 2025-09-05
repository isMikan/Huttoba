#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh			( nullptr )
	, m_pBSphere		( nullptr )
{
	m_pBSphere = new CBoundingSphere();
}

CStaticMeshObject::~CStaticMeshObject()
{
	SAFE_DELETE( m_pBSphere );
	DetachMesh();
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if( m_pMesh == nullptr ){
		return;
	}

	//中身がないなら通らない.(突貫工事)
	if(m_vRotation != D3DXVECTOR3(0.f,0.f,0.f))
	{
		//オイラー角をクォータニオンに変換.
		D3DXQuaternionRotationYawPitchRoll(
			&m_vQuaternion, m_vRotation.x, m_vRotation.y, m_vRotation.z);
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetScale( m_vScale );
	m_pMesh->SetQuaternion( m_vQuaternion );

	//CStaticMeshに現在の色を渡す.
	m_pMesh->SetDiffuseColor(m_Diffuse);
	m_pMesh->SetAmbientColor(m_Ambient);
	m_pMesh->SetSpecularColor(m_Specular);

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}