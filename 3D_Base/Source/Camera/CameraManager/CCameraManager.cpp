#include "CCameraManager.h"

CCameraManager::CCameraManager()
	: m_pCamera			( std::make_unique<CCamera>() )

	, m_Camera			()
	, m_Light			()

	, m_mView			()
	, m_mProj			()
{
	//地面の数分位置を変えるので、地面の最大数を設定.
	m_LastCamera.resize(Ground_Max);
	m_LastCamera[0] = { D3DXVECTOR3(0.f, 13.f, 0.f), D3DXVECTOR3(0.f, 0.f, 9.f) };
	m_LastCamera[1] = { D3DXVECTOR3(0.f, 14.f, 0.f), D3DXVECTOR3(0.f, 1.f, 9.f) };
	m_LastCamera[2] = { D3DXVECTOR3(0.f, 16.f, -5.f), D3DXVECTOR3(0.f, 2.f, 6.f) };
	m_LastCamera[3] = { D3DXVECTOR3(0.f, 19.f, -10.f), D3DXVECTOR3(0.f, 2.f, 6.f) };
}

CCameraManager::~CCameraManager()
{
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CCameraManager::Update_Internal()
{
	m_pCamera->UpdateView(m_Camera, m_mView);
	m_pCamera->UpdateProjection(m_mProj);
}

void CCameraManager::PositionUpdate_Internal(CGroundManager* groundManager)
{
	//もしサイズが0の場合、処理しない.
	if (groundManager->GetGrounds().size() <= 0.f) return;

	//地面サイズと同じ配列番号を設定.
	size_t index = groundManager->GetGrounds().size() - 1;

	//地面によるカメラ位置.
	float dt = CTimeManager::GetDeltaTime();

	//位置を近づかせる.
	D3DXVec3Lerp(&m_Camera.vPosition, &m_Camera.vPosition,
		&m_LastCamera[index].vPosition, dt);
	//注視点を近づかせる.
	D3DXVec3Lerp(&m_Camera.vLook, &m_Camera.vLook,
		&m_LastCamera[index].vLook, dt);
}
