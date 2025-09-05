#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh			( nullptr )
	, m_pBSphere		( nullptr )

	, m_Diffuse			( 1.0f, 1.0f, 1.0f, 1.0f )	//デフォルト色 白.
	, m_Ambient			( 0.5f, 0.5f, 0.5f, 1.0f )	//デフォルト色 グレー.
	, m_Specular		( 1.0f, 1.0f, 1.0f, 1.0f )	//デフォルト色 白.
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

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetRotation( m_vRotation );
	m_pMesh->SetScale( m_vScale );
	m_pMesh->SetQuaternion(m_vQuaternion);

	//CStaticMeshに現在の色を渡す.
	m_pMesh->SetDiffuseColor(m_Diffuse);
	m_pMesh->SetAmbientColor(m_Ambient);
	m_pMesh->SetSpecularColor(m_Specular);

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}