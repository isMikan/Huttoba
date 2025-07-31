#include "CPlayer.h"
#include "Sound/CSoundManager.h"

#include "PlayerState/CPlayerMoveIdle.h"
#include "PlayerState/CPlayerRotationIdle.h"

#include <iostream>

CPlayer::CPlayer()
	: m_pMoveState		( nullptr )
	, m_pRotationState	( nullptr )
	, m_pActionState	( nullptr )
	, m_IsBlown			( false )
{
	m_pInput = std::make_unique<CInput>(0);

	m_pMoveState = std::make_unique<CPlayerMoveIdle>();
	m_pRotationState = std::make_unique<CPlayerRotationIdle>();
}

CPlayer::~CPlayer()
{
	m_pActionState = nullptr;
	m_pRotationState = nullptr;
	m_pMoveState = nullptr;
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
	float z = 0.f;	//zŽ².
	float x = 0.f;	//xŽ².

	if (GetAsyncKeyState(VK_UP) & 0x8000)		z += 1.f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		z -= 1.f;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		x -= 1.f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	x += 1.f;

	m_pInput->BindKey(Action::Attack, InputBinding(InputDevice::Keyboard, VK_UP));


	if(m_pInput->GetLeftSthikX() != 0 
		|| m_pInput->GetLeftSthikY() != 0)
	{
		x = m_pInput->GetLeftSthikX();
		z = m_pInput->GetLeftSthikY();
	}

	m_pMoveState->KeyInput(*this, x, z);

	m_pRotationState->KeyInput(*this, x, z);
}

void CPlayer::SetMoveState(std::unique_ptr< CPlayerMoveState> newState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Eixt(*this);
	}

	m_pMoveState = std::move(newState);

	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Enter(*this);
	}
}

void CPlayer::SetRotationState(std::unique_ptr< CPlayerMoveState> newState)
{
	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Eixt(*this);
	}

	m_pRotationState = std::move(newState);

	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Enter(*this);
	}
}
