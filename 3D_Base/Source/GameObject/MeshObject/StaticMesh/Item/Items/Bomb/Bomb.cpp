#include "stdafx.h"
#include "Bomb.h"
#include "PlayerBase/CPlayerBase.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Bomb>("Bomb"); }

Bomb::Bomb()
	: m_IsTake		( false )
	, m_PickUpTime	( 1.0f )	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt	( 0.0f )
{
	Init();
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Bomb));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::Bomb));

	SetPosition(1, 5, 2);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;
}

void Bomb::Update(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
	ItemBase::Update(playiers);
}

void Bomb::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Bomb::Spawn()
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

void Bomb::OnGround()
{
	if (GetAsyncKeyState('M') & 0x8000)
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;
		
		m_IsTake = true;
	}
}

void Bomb::Have(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion(playiers);
}

void Bomb::Use(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
}

void Bomb::Throw()
{
}

void Bomb::Destroy()
{
}

void Bomb::TakeMotion()
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

void Bomb::PossessionMotion(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
	if (!m_IsTake)
	{
		m_vPosition = playiers[0]->GetPlayerRightHand().GetPosition();
	}
}

void Bomb::UseMotion()
{
}

void Bomb::ThrowMotion()
{
}