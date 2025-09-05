#include "stdafx.h"
#include "ItemBase.h"
#include "CCharactor/CPlayer/CPlayer.h"


ItemBase::ItemBase()
	: m_State	( State::Spawn )
	, m_pPlayer	( nullptr )
	, m_tGravity(0.01f)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
}

void ItemBase::Update(std::vector<std::unique_ptr<CPlayer>>& playiers)
{
	//初期化
	if (GetAsyncKeyState('R') & 0x8000)
	{
		Init();
	}
	
	//状態によって遷移
	switch (m_State)
	{
	case ItemBase::State::None:break;
	case ItemBase::State::Spawn:	Spawn();		break;
	case ItemBase::State::OnGround: OnGround();		break;
	case ItemBase::State::Have:		Have(playiers);	break;
	case ItemBase::State::Use:		Use(playiers);	break;
	case ItemBase::State::Throw:	Throw();		break;
	case ItemBase::State::Destroy:	Destroy();		break;
	default: break;
	}

	//当たり判定の座標を更新
	UpdateBSpherePos();
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

//---------------------継承で仕方なく定義------------------------
void ItemBase::Update()
{

}

