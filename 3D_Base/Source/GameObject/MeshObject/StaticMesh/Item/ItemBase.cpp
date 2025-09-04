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

void ItemBase::Update(CPlayer* player)
{
	//‰Šú‰»
	if (GetAsyncKeyState('R') & 0x8000)
	{
		Init();
	}

	
	//ó‘Ô‚É‚æ‚Á‚Ä‘JˆÚ
	switch (m_State)
	{
	case ItemBase::State::None:break;
	case ItemBase::State::Spawn:	Spawn();		break;
	case ItemBase::State::OnGround: OnGround();		break;
	case ItemBase::State::Have:		Have(player);	break;
	case ItemBase::State::Use:		Use(player);	break;
	case ItemBase::State::Throw:	Throw();		break;
	case ItemBase::State::Destroy:	Destroy();		break;
	default: break;
	}

	//“–‚½‚è”»’è‚ÌÀ•W‚ğXV
	UpdateBSpherePos();


}

void ItemBase::Update()
{

}

void ItemBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}