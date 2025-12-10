#include "CPlayerAI_TypeA.h"

#include "Item/Items/Haetataki/Haetataki.h"
#include "Item/Items/SmashBat/SmashBat.h"
#include "Item/Items/Bomb/Bomb.h"
#include "Item/Items/Mushroom/Mushroom.h"
#include "Item/Items/Fun/Fun.h"
#include "Item/Items/Boomerang/Boomerang.h"
#include "Item/Items/TrackingRobot/TrackingRobot.h"

#include "Sound/CSoundManager.h"

CPlayerAI_TypeA::CPlayerAI_TypeA(int index)
	: CPlayerAI			( index )

	, m_IsSearchPlayer	( false )
{
}

CPlayerAI_TypeA::~CPlayerAI_TypeA()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerAI_TypeA::Update()
{
	m_Control = ActionInstruct::None;

	//比較のため、最大数に設定しておく.
	m_NearbyItems.sqrt = m_MaxSqrt;

	if (m_pItemManager && !m_pHoldingItem
		&& m_pItemManager->GetItemVectorNum() > 0)
	{
		for (auto& item : m_pItemManager->GetItems())
		{
			if (!item) continue;

			if(IsSearchItem(item.get()))
			{
				FindNearbyObject(item.get(), m_NearbyItems, IsSearchItem(item.get()));
				m_TargetDir = m_NearbyItems.dir;

				if (m_NearbyItems.sqrt < 0.5f)
				{
					m_Control = ActionInstruct::ToggleItem;
				}
			}
			else
			{
				m_IsSearchPlayer = true;
			}
		}
	}
	else
	{
		m_IsSearchPlayer = true;
	}

	//プレイヤーの散策.
	if (m_IsSearchPlayer)
	{
		//比較のため、最大数に設定しておく.
		m_NearbyPlayers.sqrt = m_MaxSqrt;
		for (auto& player : m_pPlayerManager->GetPlayer())
		{
			if (!player) continue;	//プレイヤーがいない場合、次へ
			FindNearbyObject(player.get(), m_NearbyPlayers, IsSearchPlayer(player.get()));
		}
		if (m_NearbyPlayers.sqrt > 0.5f)
		{
			m_TargetDir = m_NearbyPlayers.dir;
		}

		if (m_NearbyPlayers.sqrt < RandomFloat(4.f, 8.f)
			&& IsAnyActionState<CPlayerHoldingIdleState>()
			&& IsAnyHoldingItem<Bomb, Mushroom, Fun, Boomerang, TrackingRobot>())
		{
			m_Control = ActionInstruct::Attack;
		}
		else if (m_NearbyPlayers.sqrt < RandomFloat(0.5f, 2.5f)
			&& IsAnyActionState<CPlayerActionIdleState, CPlayerHoldingIdleState>())
		{
			m_Control = ActionInstruct::Attack;
		}
	}

	if (!m_IsHitGround)
	{
		m_TargetDir = D3DXVECTOR3(0.0f, 0.f, 10.f) - m_vPosition;
	}
	AutomaticMovement(m_TargetDir);

	CPlayerAI::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerAI_TypeA::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerAI::Draw( View, Proj, Light, Camera );
}

bool CPlayerAI_TypeA::IsSearchPlayer(CPlayerBase* player)
{
	if (player->GetPlayerID() == m_PlayerID)
	{
		return false;
	}

	if (!player->IsAboveGround())
	{
		return false;
	}

	if (player->IsAnyActionState<
		CPlayerKnockbackState,
		CPlayerFallingState,
		CPlayerPushedState>())
	{
		return false;
	}

	return true;
}

bool CPlayerAI_TypeA::IsSearchItem(ItemBase* item)
{
	if(item->GetState() != IItemObserver::State::OnGround)
	{
		return false;
	}

	return true;
}
