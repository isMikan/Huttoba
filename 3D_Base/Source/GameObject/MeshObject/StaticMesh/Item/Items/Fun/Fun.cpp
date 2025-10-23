#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>("Fun"); }

Fun::Fun()
	: m_IsTake(false)
	, m_PickUpTime(1.0f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt(0.0f)

	, m_HaveOffset()
{
	Init();

	m_HaveOffset = D3DXVECTOR3(0.0, 0.2f, 0.0f);
}

Fun::~Fun()
{
}

void Fun::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Fun));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::Fun));

	//SetPosition(2, 5, 2);
	SetPosition(1, 15, 0);

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
	if (GetAsyncKeyState('1') & 0x8000)
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
	//長押ししてたら当たり続ける
	if (GetAsyncKeyState('2') & 0x8000)
	{
		Hit(playiers);
	}
	else
	{
		//離すと所持中に変化
		m_State = ItemBase::State::Have;
	}
}

void Fun::Throw(std::unique_ptr<CPlayerManager>& playiers)
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
		m_vPosition = playiers->GetPlayer(0)->GetPlayerRightHand().GetPosition() + m_HaveOffset;
		m_vQuaternion = playiers->GetPlayer(0)->GetQuaternion();
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_State = ItemBase::State::Use;
	}
	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_State = ItemBase::State::Throw;
	}
}

void Fun::UseMotion()
{
}

void Fun::ThrowMotion()
{
}

void Fun::Hit(std::unique_ptr<CPlayerManager>& playiers)
{
	playiers->GetPlayer(1)->SetHitInfo(
		m_vPosition, playiers->GetPlayer(1)->GetPosition(),
		1,
		true, CPlayerBase::HitEvent::Pushed);
}