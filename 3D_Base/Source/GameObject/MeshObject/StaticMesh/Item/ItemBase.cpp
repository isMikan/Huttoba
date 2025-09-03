#include "stdafx.h"
#include "ItemBase.h"


ItemBase::ItemBase()
	: m_State(State::Spawn)
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
	//ó‘Ô‚É‚æ‚Á‚Ä‘JˆÚ
	switch (m_State)
	{
	case ItemBase::State::None:break;
	case ItemBase::State::Spawn:	Spawn();	break;
	case ItemBase::State::OnGround: OnGround(); break;
	case ItemBase::State::Have:		Have();		break;
	case ItemBase::State::Use:		Use();		break;
	case ItemBase::State::Throw:	Throw();	break;
	case ItemBase::State::Destroy:	Destroy();	break;
	default: break;
	}

	//“–‚½‚è”»’è‚ÌÀ•W‚ğXV
	UpdateBSpherePos();

}

void ItemBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}