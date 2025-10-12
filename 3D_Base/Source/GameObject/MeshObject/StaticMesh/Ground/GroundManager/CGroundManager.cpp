#include "CGroundManager.h"

CGroundManager::CGroundManager()
	: m_pGrounds			()

	, m_FallTime			()
{
}

CGroundManager::~CGroundManager()
{
}

//--- 構築関数 ---.
void CGroundManager::Create()
{
	m_pGrounds.resize(Ground_Max);

	for (auto& ground : m_pGrounds)
	{
		//地面クラスのインスタンス作成.
		ground = std::make_unique<CGround>();

		//地面の位置を設定.
		ground->SetPosition(0.f, -0.5f, 10.f);

		m_FallTime[FirstFallGround] = 10.f;
		m_FallTime[SecondFallGround] = 20.f;
		m_FallTime[ThirdFallGround] = 30.f;
		m_FallTime[SafeGround] = 99.f;
	}
}

//--- 読込関数 ---.
void CGroundManager::LoadData()
{
	//地面のスタティックメッシュを設定.
	m_pGrounds[FirstFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::FirstFallGround));
	m_pGrounds[SecondFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SecondFallGround));
	m_pGrounds[ThirdFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::ThirdFallGround));
	m_pGrounds[SafeGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SafeGround));
}

//--- 破棄関数 ---.
void CGroundManager::Destroy()
{
}

//--- 更新関数 ---.
void CGroundManager::Update()
{
	float t = CTimeManager::GetTotalTime();

	for (int gNo = 0; gNo < m_pGrounds.size(); gNo++)
	{
		//落ちる時間が過ぎたら.
		if (m_FallTime[gNo] <= t
			&& !m_pGrounds[gNo]->GetIsFallDown())
		{
			m_pGrounds[gNo]->SetIsFallDown(true);
			m_pGrounds[gNo]->SetIsVibrate(false);
		}
		else if (m_FallTime[gNo] - CGround::m_VibrateTime <= t
			&& !m_pGrounds[gNo]->GetIsVibrate())
		{
			m_pGrounds[gNo]->SetIsVibrate(true);
			m_pGrounds[gNo]->SetVibrateStartTime(t);
		}
		else if (m_FallTime[gNo] - CGround::m_ChangeColorTime <= t
			&& !m_pGrounds[gNo]->GetIsChangeColor())
		{
			m_pGrounds[gNo]->SetIsChangeColor(true);
			m_pGrounds[gNo]->SetChangeColorStartTime(t);
		}

		m_pGrounds[gNo]->Update();
	}
}

//--- 描画関数 ---.
void CGroundManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	for (auto& ground : m_pGrounds)
	{
		ground->Draw(View, Proj, Light, Camera);
	}
}
