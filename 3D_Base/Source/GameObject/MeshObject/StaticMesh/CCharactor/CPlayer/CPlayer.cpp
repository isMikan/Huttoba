#include "CPlayer.h"
#include "Sound/CSoundManager.h"

#include "PlayerState/DirectionalInputState/PlayerMoveState/PlayerIdle/CPlayerMoveIdle.h"
#include "PlayerState/DirectionalInputState/PlayerRotationState/PlayerRotationIdle/CPlayerRotationIdle.h"
#include "PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"

#include "PlayerState/ActionState/PlayerHandAttack/CPlayerHandAttack.h"


#include <iostream>

CPlayer::CPlayer()
	: m_pInput			( std::make_unique<CInput>( 0 ) )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdle>() )
	, m_pRotationState	( std::make_unique<CPlayerRotationIdle>() )
	, m_pActionState	( std::make_unique<CPlayerActionIdle>() )

	, m_Forward			( 0.f, 0.f, 0.f )
	, m_IsBlown			( false )
{
	SetPlayerInput();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	m_pInput->Update();
	//CGameTimer::GetInstance()->Tick();
	//float deltaTime = CGameTimer::GetInstance()->GetDeltaTime();
	//if (m_IsBlown)
	//{
	//	m_Velocity += D3DXVECTOR3(0.f, -10.f, 5.f) * deltaTime;
	//	m_vPosition += m_Velocity * deltaTime;

	//	if (m_vPosition.y <= 1.f)
	//	{
	//		m_vPosition.y = 1.f;
	//		m_IsBlown = false;
	//	}
	//}

	HandleInput();

	m_pMoveState->Update(*this);
	std::cout << "MoveState : " << m_pMoveState->GetStateName() << "\n";
	std::cout << "Player Position: X=" << m_vPosition.x << ", Y=" << m_vPosition.y << ", Z=" << m_vPosition.z << "\n";

	m_pRotationState->Update(*this);
	std::cout << "RotationState : " << m_pRotationState->GetStateName() << "\n";

	m_pActionState->Update(*this);
	std::cout << "ActionState : " << m_pActionState->GetStateName() << "\n";

	CCharacter::Update();
}

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CCharacter::Draw( View, Proj, Light, Camera );
}

//void CPlayer::BlownFrom(D3DXVECTOR3 position, float force)
//{
//	D3DXVECTOR3 dir = m_vPosition - position;
//
//	D3DXVec3Normalize(&dir, &dir);
//
//	float mass = 1.f;
//	m_Velocity = dir * (force / mass);
//
//	m_IsBlown = true;
//}

void CPlayer::HandleInput()
{
	float z = 0.f;	//z軸.
	float x = 0.f;	//x軸.

	if (m_pInput->IsRepeat(Action::MoveUp))		z += 1.f;
	if (m_pInput->IsRepeat(Action::MoveDown))	z -= 1.f;
	if (m_pInput->IsRepeat(Action::MoveLeft))	x -= 1.f;
	if (m_pInput->IsRepeat(Action::MoveRight))	x += 1.f;

	if (m_pInput->IsDown(Action::Attack))
	{
		SetActionState(std::make_unique<CPlayerHandAttack>());
	}

	if(m_pInput->IsConnect())
	{
		x = m_pInput->GetLeftSthikX();
		z = m_pInput->GetLeftSthikY();
	}

	m_pMoveState->KeyInput(*this, x, z);

	m_pRotationState->KeyInput(*this, x, z);

//	for (int key = 'A'; key <= 'Z'; key++)
//	{
//		if (GetAsyncKeyState(key) & 0x8000)
//		{
//			m_pActionState->Handle(*this, key);
//		}
//	}
}

void CPlayer::SetMoveState(std::unique_ptr< CPlayerDirectionalInputState> newState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Exit(*this);
	}

	m_pMoveState = std::move(newState);

	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Enter(*this);
	}
}

void CPlayer::SetRotationState(std::unique_ptr< CPlayerDirectionalInputState> newState)
{
	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Exit(*this);
	}

	m_pRotationState = std::move(newState);

	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Enter(*this);
	}
}

void CPlayer::SetActionState(std::unique_ptr<CActionState> newState)
{
	if (m_pActionState != nullptr)
	{
		m_pActionState->Exit(*this);
	}

	m_pActionState = std::move(newState);

	if (m_pActionState != nullptr)
	{
		m_pActionState->Enter(*this);
	}
}

void CPlayer::SetPlayerInput()
{
	m_pInput->BindKey(Action::MoveUp, InputBinding(InputDevice::Keyboard, VK_UP));
	m_pInput->BindKey(Action::MoveDown, InputBinding(InputDevice::Keyboard, VK_DOWN));
	m_pInput->BindKey(Action::MoveLeft, InputBinding(InputDevice::Keyboard, VK_LEFT));
	m_pInput->BindKey(Action::MoveRight, InputBinding(InputDevice::Keyboard, VK_RIGHT));
	m_pInput->BindKey(Action::Attack, InputBinding(InputDevice::Keyboard, 'Z'));
}

//プレイヤーの正面方向を取得.
D3DXVECTOR3 CPlayer::GetForward()
{
	float yaw = m_vRotation.y;	//y軸.

	m_Forward.x = sin(yaw);
	m_Forward.y = 0.f;
	m_Forward.z = cos(yaw);

	return m_Forward;
}

//プレイヤーの方向から位置計算の関数.
D3DXVECTOR3 CPlayer::HandPositionMath(D3DXVECTOR3 offsetPos)
{
	GetForward();

	//上方向.
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	//方向
	D3DXVECTOR3 dir;
	//外積計算.
	D3DXVec3Cross(&dir, &up, &m_Forward);
	//上記の正規化.
	D3DXVec3Normalize(&dir, &dir);

	//調整位置を計算.
	D3DXVECTOR3 offset =
		dir * offsetPos.x +
		up * offsetPos.y +
		m_Forward * offsetPos.z;

	//手の位置.
	D3DXVECTOR3 handPos = m_vPosition + offset;

	return handPos;
}
