#include "CPlayerTurnState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"

CPlayerTurnState::CPlayerTurnState(CPlayerBase& pPlayer, float x, float z)
	: CPlayerState		( pPlayer )

	, m_InputDir		( x, 0.f, z )

	, m_TurnSpeed		( D3DXToRadian( 5.f ) )
	, m_TurnDir			()
{
}

CPlayerTurnState::~CPlayerTurnState()
{
}

void CPlayerTurnState::Enter()
{
}

void CPlayerTurnState::Exit()
{
	//‰ñ“]‚µ‚Ä‚¢‚È‚¢.
	m_pPlayer.SetTurning(false);
}

void CPlayerTurnState::Update()
{
	//“ü—Í‚³‚ê‚Ä‚¢‚È‚©‚Á‚½ê‡.
	if (m_InputDir.x == 0 && m_InputDir.z == 0)
	{
		//‰ñ“]‚µ‚Ä‚¢‚È‚¢ó‘Ô‚Ö.
		m_pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>(m_pPlayer));
		return;
	}

	//ƒvƒŒƒCƒ„[‚Ìƒ[ƒJƒ‹²‚ğæ“¾.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//‰ñ“]ˆ—‚ğ‚µ‚È‚¢ê‡.
	if (IsSmallTurn(axes.forward))
	{
		//‰ñ“]‚µ‚Ä‚¢‚È‚¢ó‘Ô‚Ö.
		m_pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>(m_pPlayer));
		return;
	}

	//‰ñ“]‚µ‚Ä‚¢‚é.
	m_pPlayer.SetTurning(true);

	//ÅI‚Ì‰ñ“]•ûŒü.
	m_TurnDir = atan2f(m_InputDir.x, m_InputDir.z);

	//ƒNƒH[ƒ^ƒjƒIƒ“‚Ì‰ñ“]‚ğæ“¾.
	D3DXQUATERNION currentQuat = m_pPlayer.GetQuaternion();

	//¢ŠE²‚Ìã•ûŒü.
	D3DXVECTOR3 up(0.f, 1.f, 0.f);
	//–Ú“I‚ÌŠp“x.
	D3DXQUATERNION targetQuat;
	D3DXQuaternionRotationAxis(&targetQuat, &up, m_TurnDir);
	
	//ŒX‚­•ûŒü.
	D3DXQUATERNION tiltQuat;
	//Œ»İ‚Ì‰ñ“]Šp“x‚©‚ç–Ú•W‚Ì‰ñ“]Šp“x‚Ü‚ÅŠŠ‚ç‚©‚É‰ñ“].
	D3DXQuaternionSlerp(&tiltQuat, &currentQuat, &targetQuat, m_TurnSpeed);
	//³‹K‰».
	D3DXQuaternionNormalize(&tiltQuat, &tiltQuat);

	//ƒNƒH[ƒ^ƒjƒIƒ“‚Ì‰ñ“]‚ğİ’è.
	m_pPlayer.SetQuaternion(tiltQuat);
}

bool CPlayerTurnState::IsSmallTurn(D3DXVECTOR3 forward)
{
	//“ü—Í•ûŒü‚ğ3D‹óŠÔ‚Æ“¯‚¶‚æ‚¤‚Éİ’è.
	D3DXVECTOR3 inputDir(m_InputDir.x, 0, m_InputDir.z);
	//³‹K‰».
	D3DXVec3Normalize(&inputDir, &inputDir);


	//Šp“x·‚ğŒvZ.
	float dot = D3DXVec3Dot(&inputDir, &forward);
	dot = std::clamp(dot, -1.0f, 1.0f);
	//Šp“x·‚ÌŠp“x‚ğŒvZ.
	float angle = acosf(dot);

	//‰ñ“]—Ê‚æ‚èŠp“x‚ª¬‚³‚¢.
	return angle < m_TurnSpeed;
}
