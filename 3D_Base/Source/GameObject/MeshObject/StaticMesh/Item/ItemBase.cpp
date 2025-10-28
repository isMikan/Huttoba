#include "stdafx.h"
#include "ItemBase.h"
#include "PlayerBase/CPlayerBase.h"


ItemBase::ItemBase()
	: m_State	( State::Spawn )
	, m_pPlayer	( nullptr )
	, m_tGravity(0.01f)
	, m_IsDestroy	( false )
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
}

void ItemBase::Update()
{
	//初期化
	if (GetAsyncKeyState('R') & 0x8000)
	{
		Init();
		CollisionManager::GetInstance()->Init();
	}
	
	//状態によって遷移
	switch (m_State)
	{
	case ItemBase::State::None:						break;
	case ItemBase::State::Spawn:	Spawn();		break;
	case ItemBase::State::OnGround: OnGround();		break;
	case ItemBase::State::Have:		Have();			break;
	case ItemBase::State::Use:		Use();			break;
	case ItemBase::State::Throw:	Throw();		break;
	case ItemBase::State::Destroy:	Destroy();		break;
	default: break;
	}

	//ハエたたき限定当たり判定の座標を調整+更新
	static D3DXVECTOR3 offSet;
	offSet = {0.f,0.f,1.f};
}

void ItemBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

bool ItemBase::IsUse()
{
	if (m_State == State::Use)
	{
		return true;
	}
	return false;
}