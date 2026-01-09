#include "CGroundManager.h"

CGroundManager::CGroundManager()
	: m_pGrounds		()

	, m_SafeAreaCount	( Ground_Max )
{
	Create();
	m_FallTime = { 99.f, 45.f, 30.f, 15.f };
	//\
	m_FallTime = { 999.f, 45.f, 30.f, 30.f };

	//for (int i = 0;i < 4;i++)
	//{
	//	std::cout << m_GroundRadius[i] << std::endl;
	//}
}

CGroundManager::~CGroundManager()
{
	m_pGrounds.clear();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 読込関数 ---.
void CGroundManager::LoadData()
{
	//地面のスタティックメッシュを設定.
	m_pGrounds[SafeGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SafeGround));
	m_pGrounds[SafeGround]->SetTag(GroundTag::SafeGround);

	m_pGrounds[ThirdFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::ThirdFallGround));
	m_pGrounds[ThirdFallGround]->SetTag(GroundTag::ThirdFallGround);

	m_pGrounds[SecondFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SecondFallGround));
	m_pGrounds[SecondFallGround]->SetTag(GroundTag::SecondFallGround);

	m_pGrounds[FirstFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::FirstFallGround));
	m_pGrounds[FirstFallGround]->SetTag(GroundTag::FirstFallGround);

	ExtractMeshRadius();
}

//--- 更新関数 ---.
void CGroundManager::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	for (int gNo = 0; gNo < m_pGrounds.size(); gNo++)
	{
		if (!m_pGrounds[gNo]) continue;		//存在しない場合、次へ.


		//制限のところまできたら消す.
		if (m_pGrounds[gNo]->GetPosition().y < -30.f)
		{
			Destroy(gNo);
			return;
		}

		//落ちる時間が過ぎたら.
		if (m_FallTime[gNo] <= t)
		{
			//落ちていない場合.
			if (!m_pGrounds[gNo]->GetIsFallDown())
			{
				m_pGrounds[gNo]->SetShakeTriggerTime(t);	//揺れ始めた時間.
				m_pGrounds[gNo]->SetIsChangeColor(false);	//揺れをやめる.
				m_pGrounds[gNo]->SetIsFallDown(true);		//落ちるフラグを true.
			}
		}
		//落ちる時間より(ChangeColorTime秒)前に落ちるまで.
		else if (m_FallTime[gNo] - CGround::m_ChangeColorTime <= t
			&& !m_pGrounds[gNo]->GetIsChangeColor())
		{
			m_pGrounds[gNo]->SetChangeColorTriggerTime(t);	//色が変わり始めた時間.
			m_pGrounds[gNo]->SetIsChangeColor(true);		//色が変わるフラグを true.

			m_SafeAreaCount--;		//安全な地面の数を減らす.
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
		if (!ground) continue;		//存在しない場合、次へ.

		ground->Draw(View, Proj, Light, Camera);
	}
}

//--- メイン ---.
void CGroundManager::MainGroundCreate()
{
	Create();

	for (auto& ground : m_pGrounds)
	{
		if (!ground) continue;		//存在しない場合、次へ.

		//地面の位置を設定.
		ground->SetPosition(0.f, -1.f, 10.f);
	}
}

//--- リザルト ---.
void CGroundManager::ResultGroundCreate()
{
	Create();

	for (auto& ground : m_pGrounds)
	{
		if (!ground) continue;		//存在しない場合、次へ.
		
		//地面の位置を設定.
		ground->SetPosition(3.f, -1.f, -3.f);
	}
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

D3DXVECTOR3 CGroundManager::GetGroundCenterPos()
{
	D3DXVECTOR3 center = m_pGrounds[m_pGrounds.size() - 1]->GetPosition();

	//center.x -= m_GroundRadius[m_pGrounds.size() - 1];
	//center.z -= m_GroundRadius[m_pGrounds.size() - 1];

	return center;
}

//--- 構築関数 ---.
void CGroundManager::Create()
{
	m_pGrounds.clear();
	m_pGrounds.resize(Ground_Max);

	for (auto& ground : m_pGrounds)
	{
		if (ground) continue;		//存在する場合、次へ.
		//地面クラスのインスタンス作成.
		ground = std::make_unique<CGround>();
	}
}

//--- 破棄関数 ---.
void CGroundManager::Destroy(int index)
{
	m_pGrounds[index].reset();
	m_pGrounds.resize(m_pGrounds.size() - 1);
}

bool CGroundManager::ExtractMeshRadius()
{
	for (int i = 0;i < Ground_Max;i++)
	{
		if (!m_pGrounds[i]) continue;

		//グラウンドのメッシュ
		const std::shared_ptr<CStaticMesh> mesh = m_pGrounds[i]->GetMesh();

		if (!mesh || !mesh->GetMesh())continue;

		D3DXVECTOR3 outCenter;
		float		outRadius = 0.0f;

		//CollisionUtilityにある関数からメッシュの半径と位置読み込み
		if (CollisionUtility::CalculateBoundingSphere(mesh, outCenter, outRadius))
		{
			m_GroundRadius[i] = outRadius;
		}
		else
		{
			// 計算失敗
			return false;
		}
	}

	return true;
}

