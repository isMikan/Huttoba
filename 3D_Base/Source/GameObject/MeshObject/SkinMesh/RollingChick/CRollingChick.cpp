#include "CRollingChick.h"

CRollingChick::CRollingChick()
{
	m_vScale = { 0.9f, 0.9f,0.9f };
}

CRollingChick::~CRollingChick()
{
	DetachMesh();
}

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	
//--- 更新処理 ---.
void CRollingChick::Update()
{
	//アニメーション速度を設定.
	m_pMesh->SetAnimSpeed(0.025f);

	CSkinMeshObject::Update();
}

//--- 描画処理 ---.
void CRollingChick::Draw(
	D3DXMATRIX& view, D3DXMATRIX& proj, 
	LIGHT& light, CAMERA& camera)
{
	CSkinMeshObject::Draw(view, proj, light, camera);
}

//--- プレイヤーの位置に設定 ---.
void CRollingChick::SetPlayerPos(CPlayerBase* player)
{
	m_vPosition = player->GetPlayerHead().GetPosition();
}
