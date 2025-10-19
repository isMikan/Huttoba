#include "CGroundManager.h"

CGroundManager::CGroundManager()
	: m_pGrounds			()
{
	m_FallTime = { 10.f, 20.f, 30.f, 99.f };
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
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	for (int gNo = 0; gNo < m_pGrounds.size(); gNo++)
	{
		//なかったら次へ.
		if (!m_pGrounds[gNo]) continue;

		//制限のところまできたら消す.
		if (m_pGrounds[gNo]->GetPosition().y < -30.f)
		{
			m_pGrounds[gNo].reset();
			return;
		}

		//落ちる時間が過ぎたら.
		if (m_FallTime[gNo] <= t)
		{
			if (!m_pGrounds[gNo]->GetIsFallDown())
			{
				m_pGrounds[gNo]->SetIsChangeColor(false);
				m_pGrounds[gNo]->SetIsShake(false);
				m_pGrounds[gNo]->SetIsFallDown(true);
			}
		}
		//落ちる時間より(ShakeTime秒)前から落ちるまで.
		else if (m_FallTime[gNo] - CGround::m_ShakeTime <= t)
		{
			if (!m_pGrounds[gNo]->GetIsShake())
			{
				m_pGrounds[gNo]->SetShakeTriggerTime(t);
				m_pGrounds[gNo]->SetIsShake(true);
			}
		}
		//落ちる時間より(ChangeColorTime秒)前に落ちるまで.
		else if (m_FallTime[gNo] - CGround::m_ChangeColorTime <= t
			&& !m_pGrounds[gNo]->GetIsChangeColor())
		{
			m_pGrounds[gNo]->SetChangeColorTriggerTime(t);
			m_pGrounds[gNo]->SetIsChangeColor(true);
		}

		//m_pGrounds[gNo]->Update();
	}
}

//--- 描画関数 ---.
void CGroundManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	for (auto& ground : m_pGrounds)
	{
		//なかったら次へ.
		if (!ground) continue;

		ground->Draw(View, Proj, Light, Camera);
	}
}
