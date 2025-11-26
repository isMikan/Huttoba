#include "stdafx.h"
#include "ItemBase.h"
#include "PlayerBase/CPlayerBase.h"
#include "Collision/CollisionUtility/CollisionUtility.h"

ItemBase::ItemBase()
	: m_State		{ IItemObserver::State::Spawn }
	, m_OldState	{ IItemObserver::State::None }
	, m_pPlayer		{ nullptr }
	, m_tGravity	{ 9.8f }
	, m_IsDestroy	{ false }
	, m_IsOnGround	{ false }
	, m_IsOkFall	{ true }

	, m_ThrowSmashPower	{ 5.0f }
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
	//アイテムの状態が切り替われば
	if (m_State != m_OldState)
	{
		ItemState(m_State);

		m_OldState = m_State;
	}

	//状態によって遷移
	switch (m_State)
	{
	case IItemObserver::IItemObserver::State::None:						break;
	case IItemObserver::IItemObserver::State::Spawn:	Spawn();		break;
	case IItemObserver::IItemObserver::State::OnGround: OnGround();		break;
	case IItemObserver::IItemObserver::State::Have:		Have();			break;
	case IItemObserver::IItemObserver::State::Use:		Use();			break;
	case IItemObserver::IItemObserver::State::Throw:	Throw();		break;
	case IItemObserver::IItemObserver::State::Destroy:	Destroy();		break;
	default: break;
	}

	//ハエたたき限定当たり判定の座標を調整+更新
	static D3DXVECTOR3 offSet;
	offSet = {0.f,0.f,1.f};

	static constexpr float UNDER_MAX = -5.f;
	if (m_vPosition.y < UNDER_MAX)
	{
		m_State = IItemObserver::State::Destroy;
	}

}

void ItemBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

bool ItemBase::IsUse()
{
	if (m_State == IItemObserver::State::Use)
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

void ItemBase::DestroyItem()
{
	m_State = IItemObserver::State::Destroy;
}

void ItemBase::Fall()
{
	static constexpr float GRAVITY = 0.1f;

	m_vPosition.y -= GRAVITY;
}

void ItemBase::OnCollision(CollisionBase* other)
{

}

void ItemBase::ItemState(IItemObserver::State state)
{
}

void ItemBase::ThrowSmash(CPlayerBase& playiers)
{
	//プレイヤーの吹き飛ばしの計算
	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, m_ThrowSmashPower, 50.0f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);
}
