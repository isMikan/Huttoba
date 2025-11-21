#include "CPlayerAI_TypeA.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerItemAttackState/CPlayerItemAttackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

#include "Sound/CSoundManager.h"

CPlayerAI_TypeA::CPlayerAI_TypeA(int index)
	: CPlayerAI		( index )
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
	for (auto& item : m_pItemManager->GetItems())
	{
		if (!m_pItemBase && IsSearchItem(item.get()))
		{
			FindNearbyObject(item.get(), m_NearbyItems, IsSearchItem(item.get()));
			AutomaticMovement(m_NearbyItems.dir);

			if (m_NearbyItems.sqrt < 0.5f)
			{
				m_Control = ActionInstruct::ToggleItem;
				std::cout << "拾う" << std::endl;
			}
			std::cout << "アイテム" << std::endl;
		}
		else
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
				AutomaticMovement(m_NearbyPlayers.dir);
			}
			std::cout << "プレイヤー" << std::endl;
			if (m_NearbyPlayers.sqrt < RandomFloat(2.f, 6.f)
				&& (IsAnyActionState<CPlayerActionIdleState>()
				|| IsAnyActionState<CPlayerHoldingIdleState>()))
			{
				m_Control = ActionInstruct::Attack;
				std::cout << "攻撃" << std::endl;
			}
		}
	}

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
	if(item->GetState() != ItemBase::State::OnGround)
	{
		return false;
	}
}
