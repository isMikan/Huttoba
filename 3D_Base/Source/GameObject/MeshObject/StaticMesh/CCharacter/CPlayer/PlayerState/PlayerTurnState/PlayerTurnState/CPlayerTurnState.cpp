#include "CPlayerTurnState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"

CPlayerTurnState::CPlayerTurnState(float x, float z)
	:  m_Dir			( x, 0.f, z )
	, m_TurnSpeed		( D3DXToRadian( 5.f ) )
	, m_TurnDir			()
{
}

CPlayerTurnState::~CPlayerTurnState()
{
}

void CPlayerTurnState::Enter(CPlayer& pPlayer)
{
}

void CPlayerTurnState::Exit(CPlayer& pPlayer)
{
	pPlayer.SetRotation(pPlayer.GetRotation());
}

void CPlayerTurnState::Update(CPlayer& pPlayer)
{
	if (m_Dir.x == 0 && m_Dir.z == 0)
	{
		pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>());
		return;
	}

	//最終の回転方向.
	m_TurnDir = atan2f(m_Dir.x, m_Dir.z);

	//回転しているかを true に設定.
	pPlayer.SetRotating(true);

	//ローカル軸の取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//クォータニオンの回転を取得.
	D3DXQUATERNION currentQuat = pPlayer.GetQuaternion();

	//目標の角度.
	D3DXQUATERNION targetQuat;
	D3DXQuaternionRotationAxis(&targetQuat, &axes.up, m_TurnDir);

	//現在傾く角度.
	D3DXQUATERNION quat;
	//今の回転角度から目標の回転角度まで滑らかに回転.
	D3DXQuaternionSlerp(&quat, &currentQuat, &targetQuat, m_TurnSpeed);
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの回転を設定.
	pPlayer.SetQuaternion(quat);
}