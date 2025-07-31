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
	m_pMoveState = new CPlayerMoveIdle();
	m_pRotationState = new CPlayerRotationIdle();
}

CPlayer::~CPlayer()
{
	m_pActionState = nullptr;
	m_pRotationState = nullptr;
	m_pMoveState = nullptr;
}

void CPlayer::Update()
{
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

	//if (!m_IsBlown)
	//{
#if 1
		//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		//	m_vRotation.y += m_TurnSpeed;
		//}
		//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		//	m_vRotation.y -= m_TurnSpeed;
		//}

		//RadioControl();
#else
		float add_value = 0.1f;
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			m_vPosition.y += add_value;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			m_vPosition.y -= add_value;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			m_vPosition.x += add_value;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			m_vPosition.x -= add_value;
		}
#endif

		////‘O‰ñ‚ÌƒtƒŒ[ƒ€‚Å’e‚ð”ò‚Î‚µ‚Ä‚¢‚é‚©‚à’m‚ê‚È‚¢‚Ì‚Åfalse‚É‚·‚é.
		//m_Shot = false;

		////’e‚ð”ò‚Î‚µ‚½‚¢!
		//if (GetAsyncKeyState('Z') & 0x8000)
		//{
		//	m_Shot = true;

		//	//SE‚ÌÄ¶.
		//	CSoundManager::PlaySE(CSoundManager::SE_Jump);
		//}
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

	m_pMoveState->KeyInput(*this, x, z);

	m_pRotationState->KeyInput(*this, x, z);
}

void CPlayer::SetMoveState(CPlayerMoveState* newState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Eixt(*this);
		delete m_pMoveState;
	}

	m_pMoveState = newState;

	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Enter(*this);
	}
}

void CPlayer::SetRotationState(CPlayerMoveState* newState)
{
	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Eixt(*this);
		delete m_pRotationState;
	}

	m_pRotationState = newState;

	if (m_pRotationState != nullptr)
	{
		m_pRotationState->Enter(*this);
	}
}
