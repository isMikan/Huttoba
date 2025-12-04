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


#include "Sound/CSoundManager.h"

CPlayerAI::CPlayerAI(int index)
	: CPlayerBase		( index )

	, m_pPlayerManager	( nullptr )
	, m_pItemManager	( nullptr )

	, m_CurrentDir		( 0.f, 0.f, 0.f )
	, m_PreviousDiff	( 99.f, 99.f, 99.f )	//初回は、比較のため大きい数値にしておく.

	, m_NearbyPlayers	()
	, m_NearbyItems		()

	, m_MaxSqrt			( 999.f )	//初回は、比較のため大きい数値にしておく.
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

//======================================================================
//		内部で呼び出す関数.
//======================================================================

//--- 移動処理 ---.
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
		//入力に変化があった場合.
		if (m_CurrentDir != targetDir)
		{
			//回転だけしない場合.
			if (IsAnyActionState<CPlayerFallingState>())	//落ちている.
			{
				SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));
			}
			//移動・回転する場合.
			else
			{
				SetTurnState(std::make_unique<CPlayerTurnState>(*this, targetDir.x, targetDir.z));
			}
			m_CurrentDir = targetDir;	//現在の入力を記録しておく.
		}

		SetMoveState(std::make_unique<CPlayerMoveState>(*this, targetDir.x, targetDir.z));
	}
}

//--- ランダム数値 ---.
float CPlayerAI::RandomFloat(float min, float max)
{
	//乱数の初期シードを作成.
	static std::random_device rd;
	//乱数エンジンを初期化.
	static std::mt19937 mt(rd());
	//指定した範囲の float 値を求める.
	std::uniform_real_distribution<float> dist(min, max);

	return dist(mt);
}
