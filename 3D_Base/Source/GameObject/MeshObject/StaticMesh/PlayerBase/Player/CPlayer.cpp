#include "CPlayer.h"


#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"
#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerPickupState/CPlayerPickupState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerThrowState/CPlayerThrowState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"

#include "Input/CInputManager.h"
#include "Sound/CSoundManager.h"

#include <unordered_map>
#include <array>

CPlayer::CPlayer(int index)
	: CPlayerBase			( index )

	, m_PlayerID			( index )
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

	if(m_PlayerEvent == PlayerEvent::HandWhiff
		|| m_PlayerEvent == PlayerEvent::Knockback
		|| m_PlayerEvent == PlayerEvent::Getup
		|| m_PlayerEvent == PlayerEvent::Knockdown)
	{
		SetMoveState(std::make_unique<CPlayerMoveIdleState>(*this));
		SetTurnState(std::make_unique<CPlayerTurnIdleState>(*this));
	}
	else
	{
		SetMoveState(std::make_unique<CPlayerMoveState>(*this, x, z));
		SetTurnState(std::make_unique<CPlayerTurnState>(*this, x, z));
	}

	//アイテムを持っていないなら攻撃.
	if (CInputManager::IsDown(Action::Attack, m_PlayerID)
		&& !m_IsHoldingItem
		&& m_PlayerEvent != PlayerEvent::HandAttack
		&& m_PlayerEvent != PlayerEvent::HandWhiff)
	{
		SetActionState(std::make_unique<CPlayerHandAttackState>(*this));
	}
	//アイテムを持っていないなら拾う.
	if (CInputManager::IsDown(Action::ToggleItem, m_PlayerID) && !m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerPickupState>(*this));
	}
	//アイテムを持っているなら捨てる.
	else if (CInputManager::IsDown(Action::ToggleItem, m_PlayerID) && m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerThrowState>(*this));
	}
	//押された時の処理.
	if (m_HitInfo.isHit
		&& m_HitInfo.hitEvent == HitEvent::Pushed)
	{
		SetActionState(std::make_unique<CPlayerPushedState>(*this));
	}
}

//--- キャラクターの色を設定する関数 ---.
void CPlayer::SetCharacterDefault(int index)
{
	std::array<D3DXVECTOR3, Player_Max> playerPos;							//プレイヤーの位置.
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor;	//プレイヤーの色.

	switch (index)
	{
	case 0:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 5.f);
		playerColor[index] = {
			D3DXVECTOR4(1.f, 0.f, 0.f, 1.f), // 赤
			D3DXVECTOR4(0.5f, 0.f, 0.f, 1.f), // 少し暗めの赤
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),
		};

		break;
	case 1:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 5.f);
		playerColor[index] = {
			D3DXVECTOR4(0.f, 0.f, 1.f, 1.f),  // 青
			D3DXVECTOR4(0.f, 0.f, 0.5f, 1.f),  // 少し暗めの青
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 2:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 10.f);
		playerColor[index] = {
			D3DXVECTOR4(1.0f, 0.5f, 0.f, 1.f),  // オレンジ
			D3DXVECTOR4(0.5f, 0.3f, 0.f, 1.f), // 少し暗めのオレンジ
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 3:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 10.f);
		playerColor[index] = {
			D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.f),  // 緑
			D3DXVECTOR4(0.0f, 0.5f, 0.0f, 1.f),  // 少し暗めの緑
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	default:
		break;
	}

	m_vPosition = playerPos[index];
	m_ObjColor = playerColor[index];
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
			keyMap
			{
				{Action::MoveUp,		'T'},	//上移動.
				{Action::MoveDown,		'G'},	//下移動.
				{Action::MoveLeft,		'F'},	//左移動.
				{Action::MoveRight,		'H'},	//右移動.
				{Action::Attack,		'R'}, 	//攻撃.
				{Action::ToggleItem,	'Y'}, 	//拾う/捨てる.
			},
			//プレイヤー3.
			keyMap
			{
				{Action::MoveUp,		'I'},	//上移動.
				{Action::MoveDown,		'K'},	//下移動.
				{Action::MoveLeft,		'J'},	//左移動.
				{Action::MoveRight,		'L'},	//右移動.
				{Action::Attack,		'U'}, 	//攻撃.
				{Action::ToggleItem,	'O'}, 	//拾う/捨てる.
			},
			//プレイヤー4.
			keyMap
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
