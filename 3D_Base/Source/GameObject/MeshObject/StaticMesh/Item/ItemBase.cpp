#include "stdafx.h"
#include "ItemBase.h"
#include "PlayerBase/CPlayerBase.h"
#include "Collision/CollisionUtility/CollisionUtility.h"

ItemBase::ItemBase()
	: m_State		{ State::Spawn }
	, m_pPlayer		{ nullptr }
	, m_tGravity	{ 9.8f }
	, m_IsDestroy	{ false }
	, m_IsOnGround	{ false }
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

	static constexpr float UNDER_MAX = -5.f;
	if (m_vPosition.y < UNDER_MAX)
	{
		m_IsDestroy = true;
	}
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

void ItemBase::IsOnGround(CGroundManager& pGroundMgr)
{
	float groundY = 0.0f;

	// 自身の位置を取得
	const D3DXVECTOR3 playerPos = GetPosition();

	// サイズ決定(後で定数に突貫)
	const float playerHalfHeight = 0.4f;

	// CollisionManagerに判定を依頼し、結果をそのまま返す
	m_IsOnGround = CollisionUtility::CheckGroundContact(
		playerPos,
		pGroundMgr,
		groundY
	);
}

void ItemBase::Fall()
{
	static constexpr float GRAVITY = 0.1f;

	m_vPosition.y -= GRAVITY;
}
