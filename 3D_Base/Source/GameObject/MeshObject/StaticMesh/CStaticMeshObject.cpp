#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh			( nullptr )
{
	//初期色.
	ObjectColor color = {
		D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f),
		D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.f),
		D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f) };
	//サイズを指定する.
	m_ObjectColor.resize(1);
	//色を設定.
	m_ObjectColor[0] = color;
}

CStaticMeshObject::~CStaticMeshObject()
{
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

	for (int mNo = 0; mNo < m_ObjectColor.size(); mNo++)
	{
		//CStaticMeshに現在の色を渡す.
		m_pMesh->SetObjectColor(
			mNo, m_ObjectColor[mNo].diffuse, m_ObjectColor[mNo].ambient, m_ObjectColor[mNo].specular);
	}

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}