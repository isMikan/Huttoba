#include "CPlayerAI_TypeA.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"
#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
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
	for(int pNo = 0; pNo < Player_Max; pNo++)
	{
		const auto& player = m_pPlayerManager->GetPlayer(pNo);

		if (pNo == m_PlayerID) continue;
		FollowPlayerNearby();
	}

	CPlayerAI::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerAI_TypeA::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerAI::Draw( View, Proj, Light, Camera );
}