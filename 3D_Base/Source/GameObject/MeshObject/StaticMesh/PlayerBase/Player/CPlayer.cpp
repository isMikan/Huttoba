#include "CPlayer.h"


#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"
#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerPickupState/CPlayerPickupState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerThrowState/CPlayerThrowState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

#include "Input/CInputManager.h"
#include "Sound/CSoundManager.h"

#include <unordered_map>
#include <array>

CPlayer::CPlayer(int index)
	: CPlayerBase			( index )
{
	SetPlayerInputBinding(m_PlayerID);
}

CPlayer::~CPlayer()
{
}

//--- 毎フレームの動作する関数 ---.
void CPlayer::Update()
{
	HandleInput();

	CPlayerBase::Update();
}

//--- 毎フレームの描画する関数 ---.
void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerBase::Draw( View, Proj, Light, Camera );
}

//--- キー操作 ---.
void CPlayer::HandleInput()
{
	float x = 0.f;	//x軸.
	float z = 0.f;	//z軸.

	//移動回転をしない場合.
	if (IsAnyActionState<
		CPlayerHandWhiffState,
		CPlayerKnockbackState,
		CPlayerGetUpState,
		CPlayerKnockdownState>())
	{
		SetMoveState(std::make_unique<CPlayerMoveIdleState>(*this));
		SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));
	}
	else
	{
		if (CInputManager::IsRepeat(Action::MoveUp, m_PlayerID))	z += 1.f;
		if (CInputManager::IsRepeat(Action::MoveDown, m_PlayerID))	z -= 1.f;
		if (CInputManager::IsRepeat(Action::MoveLeft, m_PlayerID))	x -= 1.f;
		if (CInputManager::IsRepeat(Action::MoveRight, m_PlayerID))	x += 1.f;

		//接続されていたら数値を受け取る.
		if (CInputManager::IsConnect(m_PlayerID))
		{
			x = CInputManager::GetLeftSthikX(m_PlayerID);
			z = CInputManager::GetLeftSthikY(m_PlayerID);
		}
	}

	//移動だけする場合.
	SetMoveState(std::make_unique<CPlayerMoveState>(*this, x, z));
	//回転だけしない場合.
	if (IsActionState<CPlayerFallingState>())
	{
		SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));
	}
	//移動回転する場合.
	else
	{
		SetTurnState(std::make_unique<CPlayerTurnState>(*this, x, z));
	}

	//アイテムを持っていないなら攻撃.
	if (CInputManager::IsDown(Action::Attack, m_PlayerID)
		&& !m_IsHoldingItem
		&& IsActionState<CPlayerActionIdleState>())
	{
		SetActionState(std::make_unique<CPlayerHandAttackState>(*this));
	}
	//アイテムを持っていないなら拾う.
	if (CInputManager::IsDown(Action::ToggleItem, m_PlayerID)
		&& !m_IsHoldingItem
		&& IsActionState<CPlayerActionIdleState>())
	{
		SetActionState(std::make_unique<CPlayerPickupState>(*this));
	}
	//アイテムを持っているなら捨てる.
	else if (CInputManager::IsDown(Action::ToggleItem, m_PlayerID)
		&& m_IsHoldingItem
		&& IsActionState<CPlayerActionIdleState>())
	{
		SetActionState(std::make_unique<CPlayerThrowState>(*this));
	}
}

//--- キーバインドを設定する関数 ---.
void CPlayer::SetPlayerInputBinding(int index) const
{
	//キーボード操作.
	{
		//プレイヤーごとにキーを設定するため.
		using keyMap = std::unordered_map<Action, int>;
		//プレイヤー数分にキーを割り当てる.
		static const std::array<keyMap, Player_Max> keys =
		{
			//プレイヤー1.
			keyMap
			{
				{Action::MoveUp,		'W'},	//上移動.
				{Action::MoveDown,		'S'},	//下移動.
				{Action::MoveLeft,		'A'},	//左移動.
				{Action::MoveRight,		'D'},	//右移動.
				{Action::Attack,		'Q'},	//攻撃.
				{Action::ToggleItem,	'E'},	//拾う/捨てる.
			},
			//プレイヤー2.
			{
				{Action::MoveUp,		'T'},	//上移動.
				{Action::MoveDown,		'G'},	//下移動.
				{Action::MoveLeft,		'F'},	//左移動.
				{Action::MoveRight,		'H'},	//右移動.
				{Action::Attack,		'R'}, 	//攻撃.
				{Action::ToggleItem,	'Y'}, 	//拾う/捨てる.
			},
			//プレイヤー3.
			{
				{Action::MoveUp,		'I'},	//上移動.
				{Action::MoveDown,		'K'},	//下移動.
				{Action::MoveLeft,		'J'},	//左移動.
				{Action::MoveRight,		'L'},	//右移動.
				{Action::Attack,		'U'}, 	//攻撃.
				{Action::ToggleItem,	'O'}, 	//拾う/捨てる.
			},
			//プレイヤー4.
			{
				{Action::MoveUp,		VK_OEM_3},		//上移動.
				{Action::MoveDown,		VK_OEM_1},		//下移動.
				{Action::MoveLeft,		VK_OEM_PLUS},	//左移動.
				{Action::MoveRight,		VK_OEM_6},		//右移動.
				{Action::Attack,		'P'},			//攻撃.
				{Action::ToggleItem,	VK_OEM_4},		//拾う/捨てる.
			},
		};

		//プレイヤーにキーを設定.
		for (const auto& key : keys[index])
		{
			const Action action = key.first;
			const int code = key.second;

			CInputManager::BindKey(
				action, InputBinding(InputDevice::Keyboard, code), m_PlayerID);
		}
	}

	//コントローラ操作.
	CInputManager::BindKey(Action::Attack,
		InputBinding(InputDevice::GamePad, CXInput::B), m_PlayerID);	//攻撃.
	CInputManager::BindKey(Action::ToggleItem,	
		InputBinding(InputDevice::GamePad, CXInput::A), m_PlayerID);	//拾う/捨てる.
}

void CPlayer::OnCollision(CollisionBase* pOtherCollider)
{
//	//攻撃を受けるプレイヤー.
//	for (int hNo = 0;hNo < Player_Max;hNo++)
//	{
//		//攻撃するプレイヤー.
//		for (int aNo = 0;aNo < Player_Max;aNo++)
//		{
//			if (hNo == aNo) continue;
//			{
//				switch (hNo)
//				{
//				case 2:
//					m_pPlayers[hNo]->SetHitInfo(
//						m_pPlayers[aNo]->GetPosition(), true, CPlayerBase::HitEvent::Pushed);
//					break;
//				case 3:
//					break;
//				default:
//					m_pPlayers[hNo]->SetHitInfo(
//						m_pPlayers[aNo]->GetPosition(), m_pPlayers[aNo]->GetPosition(), 10.f, true, CPlayerBase::HitEvent::Knockdown);
//					break;
//				}
//
//				m_pPlayers[aNo]->SetHitInfo(
//					m_pPlayers[aNo]->GetPosition(), true, CPlayerBase::HitEvent::None);
//			}
//		}
//	}
	// 衝突相手のタグをチェックし、応答を切り替える
	switch (pOtherCollider->GetTag())
	{
	case CollisionBase::ColliderTag::Player:
		if (IsAnyActionState<CPlayerHandAttackState>())
		{
			SetHitInfo(
				pOtherCollider->GetWorldPosition(), pOtherCollider->GetWorldPosition(), 10.f, true, CPlayerBase::HitEvent::Knockdown);
		}
		break;
	}
}