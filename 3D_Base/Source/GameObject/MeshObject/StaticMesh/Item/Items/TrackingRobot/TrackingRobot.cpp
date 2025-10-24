#include "TrackingRobot.h"

#include "stdafx.h"
#include "PlayerBase/CPlayerBase.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<TrackingRobot>("TrackingRobot"); }

TrackingRobot::TrackingRobot()
	: m_IsTake(false)
	, m_PickUpTime(1.0f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt(0.0f)
{
	Init();
}

TrackingRobot::~TrackingRobot()
{
}

void TrackingRobot::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::TrackingRobot));

	//SetPosition(4, 5, 2);
	SetPosition(0, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;
}

void TrackingRobot::Update(std::unique_ptr<CPlayerManager>& playiers)
{
	ItemBase::Update(playiers);
}

void TrackingRobot::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void TrackingRobot::Spawn()
{
	//落下処理
	if (m_vPosition.y > 1.2)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += 0.001f;
	}
	else
	{
		//状態を地面についたときに変更
		m_State = ItemBase::State::OnGround;
	}
}

void TrackingRobot::OnGround()
{
	if (GetAsyncKeyState('M') & 0x8000)
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		m_IsTake = true;
	}
}

void TrackingRobot::Have(std::unique_ptr<CPlayerManager>& playiers)
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion(playiers);
}

void TrackingRobot::Use(std::unique_ptr<CPlayerManager>& playiers)
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		//m_pPlayer->GetVelocity();
	}
}

void TrackingRobot::Throw(std::unique_ptr<CPlayerManager>& playiers)
{
}

void TrackingRobot::Destroy()
{
}

void TrackingRobot::TakeMotion()
{
	if (m_IsTake)
	{
		m_PickUpCnt += CTimeManager::GetDeltaTime();

		if (m_PickUpCnt >= m_PickUpTime)
		{
			m_IsTake = false;
		}
	}
}

void TrackingRobot::PossessionMotion(std::unique_ptr<CPlayerManager>& playiers)
{
	if (!m_IsTake)
	{
		//m_vPosition = playiers[0]->GetPlayerRightHand().GetPosition();
	}
}

void TrackingRobot::UseMotion()
{
}

void TrackingRobot::ThrowMotion()
{
}