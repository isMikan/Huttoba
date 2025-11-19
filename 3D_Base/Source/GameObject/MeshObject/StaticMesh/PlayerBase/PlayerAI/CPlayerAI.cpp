#include "CPlayerAI.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "Sound/CSoundManager.h"

CPlayerAI::CPlayerAI(int index)
	: CPlayerBase		( index )

	, m_pPlayerManager	( nullptr )
	, m_pItemManager	( nullptr )

	, m_CurrentDir		( 0.f, 0.f, 0.f )
	, m_PreviousDiff	( 99.f, 99.f, 99.f )	//初回は、比較のため大きい数値にしておく.

	, m_NearbyPlayers	()
	, m_NearbyItems		()

	, m_Sqrt			( 999.f )	//初回は、比較のため大きい数値にしておく.
{
}

CPlayerAI::~CPlayerAI()
{
	m_pPlayerManager = nullptr;
	m_pItemManager = nullptr;
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerAI::Update()
{
	CPlayerBase::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerAI::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerBase::Draw( View, Proj, Light, Camera );
}

void CPlayerAI::AutomaticMovement(D3DXVECTOR3 targetDir)
{
	//移動・回転をしない場合.
	if (IsAnyActionState<
		CPlayerHandWhiffState,		//ふらつき.
		CPlayerKnockbackState,		//吹き飛ばされ.
		CPlayerGetUpState,			//起き上がり.
		CPlayerKnockdownState>())	//ダウン中.
	{
		//入力に変化があった場合.
		if (m_CurrentDir != targetDir)
		{
			SetMoveState(std::make_unique<CPlayerMoveIdleState>(*this));
			SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));

			m_CurrentDir = targetDir;	//現在の入力を記録しておく.
		}
	}
	else
	{
		//回転だけしない場合.
		if (IsAnyActionState<CPlayerFallingState>())	//落ちている.
		{
			//入力に変化があった場合.
			if (m_CurrentDir != targetDir)
			{
				SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));
			}
		}
		//移動・回転する場合.
		else
		{
			//入力に変化があった場合.
			if (m_CurrentDir != targetDir)
			{
				SetTurnState(std::make_unique<CPlayerTurnState>(*this, targetDir.x, targetDir.z));
			}
		}

		//入力に変化があった場合.
		if (m_CurrentDir != targetDir)
		{
			SetMoveState(std::make_unique<CPlayerMoveState>(*this, targetDir.x, targetDir.z));
			m_CurrentDir = targetDir;	//現在の入力を記録しておく.
		}
	}
}

//--- 近くのプレイヤーを探索 ---.
void CPlayerAI::FindNearbyPlayers()
{
	m_NearbyPlayers.sqrt = m_Sqrt;
	D3DXVECTOR3 nearestDir(0.f, 0.f, 0.f);

	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		if (pNo == m_PlayerID) continue;

		const auto& player = m_pPlayerManager->GetPlayer(pNo);

		if (!player || !player->IsAboveGround()) continue;

		if (player->IsAnyActionState<
			CPlayerKnockbackState,
			CPlayerFallingState,
			CPlayerPushedState>()) continue;

		D3DXVECTOR3 playerPos = m_pPlayerManager->GetPlayer(pNo)->GetPosition();

		D3DXVECTOR3 diff = playerPos - m_vPosition;
		float diffSqrt = D3DXVec3LengthSq(&diff);
		
		if (diffSqrt < m_NearbyPlayers.sqrt)
		{
			m_NearbyPlayers.sqrt = diffSqrt;
			D3DXVec3Normalize(&nearestDir, &diff);
		}

		if (m_NearbyPlayers.sqrt < m_Sqrt)
		{
			m_NearbyPlayers.dir = nearestDir;
		}
	}
}

//--- 近くのアイテムを探索 ---.
void CPlayerAI::FindNearbyItems()
{
	m_NearbyItems.sqrt = m_Sqrt;
	D3DXVECTOR3 nearestDir(0.f, 0.f, 0.f);

	for (int iNo = 0; iNo < m_pItemManager->GetItemVectorNum(); iNo++)
	{
		D3DXVECTOR3 itemPos = m_pItemManager->GetItemPos(iNo);

		D3DXVECTOR3 diff = itemPos - m_vPosition;
		float diffSqrt = D3DXVec3LengthSq(&diff);

		if (diffSqrt < m_NearbyItems.sqrt)
		{
			m_NearbyItems.sqrt = diffSqrt;
			D3DXVec3Normalize(&nearestDir, &diff);
		}

		if (m_NearbyItems.sqrt < m_Sqrt)
		{
			m_NearbyItems.dir = nearestDir;
		}
	}
}
