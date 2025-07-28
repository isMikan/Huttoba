#include "CPlayer.h"
#include "Assets/Sound/CSoundManager.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	float add_value = 0.1f;
	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		m_vPosition.y += add_value;
	}
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		m_vPosition.y -= add_value;
	}
	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		m_vPosition.x += add_value;
	}
	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		m_vPosition.x -= add_value;
	}
	CCharacter::Update();
}

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw( View, Proj, Light, Camera );
}