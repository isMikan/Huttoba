#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/CPlayerBase.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>("Fun"); }

Fun::Fun()
	: m_IsTake(false)
	, m_PickUpTime(1.0f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt(0.0f)
{
	Init();
}

Fun::~Fun()
{
}

void Fun::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Fun));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::Fun));

	//SetPosition(2, 5, 2);
	SetPosition(0, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;
}

void Fun::Update(std::unique_ptr<CPlayerManager>& playiers)
{
	ItemBase::Update(playiers);
}

void Fun::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Fun::Spawn()
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

void Fun::OnGround()
{
	if (GetAsyncKeyState('M') & 0x8000)
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		m_IsTake = true;
	}
}

void Fun::Have(std::unique_ptr<CPlayerManager>& playiers)
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion(playiers);
}

void Fun::Use(std::unique_ptr<CPlayerManager>& playiers)
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		//m_pPlayer->GetVelocity();
	}
}

void Fun::Throw()
{
}

void Fun::Destroy()
{
}

void Fun::TakeMotion()
{
	if (m_IsTake)
	{
		m_PickUpCnt += CTimeManager::GetDeltaTime();
		m_vPosition.x += 0.1;
		m_vPosition.z += 0.1;

		if (m_PickUpCnt >= m_PickUpTime)
		{
			m_IsTake = false;
		}
	}
}

void Fun::PossessionMotion(std::unique_ptr<CPlayerManager>& playiers)
{
	if (!m_IsTake)
	{
		//m_vPosition = playiers[0]->GetPlayerRightHand().GetPosition();
	}
}

void Fun::UseMotion()
{
}

void Fun::ThrowMotion()
{
}