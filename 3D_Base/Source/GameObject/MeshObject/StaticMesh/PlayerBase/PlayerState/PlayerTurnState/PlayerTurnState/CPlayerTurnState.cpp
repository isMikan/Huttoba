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

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerTurnState::Enter()
{
}

//--- 状態の終了時に呼び出す ---.
void CPlayerTurnState::Exit()
{
	//回転していない.
	m_pPlayer.SetTurning(false);
}
	
//--- この状態の間に呼び出す ---.
void CPlayerTurnState::Update()
{
	//入力されていなかった場合.
	if (m_InputDir.x == 0 && m_InputDir.z == 0)
	{
		//回転していない状態へ.
		m_pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>(m_pPlayer));
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//回転処理をしない場合.
	if (IsSmallTurn(axes.forward))
	{
		//回転していない状態へ.
		m_pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>(m_pPlayer));
		return;
	}

	//回転している.
	m_pPlayer.SetTurning(true);

	//最終の回転方向.
	m_TurnDir = atan2f(m_InputDir.x, m_InputDir.z);

	//クォータニオンの回転を取得.
	D3DXQUATERNION currentQuat = m_pPlayer.GetQuaternion();

	//世界軸の上方向.
	D3DXVECTOR3 up(0.f, 1.f, 0.f);
	//目的の角度.
	D3DXQUATERNION targetQuat;
	D3DXQuaternionRotationAxis(&targetQuat, &up, m_TurnDir);
	
	//傾く方向.
	D3DXQUATERNION tiltQuat;
	//現在の回転角度から目標の回転角度まで滑らかに回転.
	D3DXQuaternionSlerp(&tiltQuat, &currentQuat, &targetQuat, m_TurnSpeed);
	//正規化.
	D3DXQuaternionNormalize(&tiltQuat, &tiltQuat);

	//クォータニオンの回転を設定.
	m_pPlayer.SetQuaternion(tiltQuat);
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- 目的の方向に達成している場合 ---.
bool CPlayerTurnState::IsSmallTurn(D3DXVECTOR3 forward)
{
	//入力方向を3D空間と同じように設定.
	D3DXVECTOR3 inputDir(m_InputDir.x, 0, m_InputDir.z);
	//正規化.
	D3DXVec3Normalize(&inputDir, &inputDir);

	//角度差を計算.
	float dot = D3DXVec3Dot(&inputDir, &forward);
	dot = std::clamp(dot, -1.0f, 1.0f);
	//角度差の角度を計算.
	float angle = acosf(dot);

	//回転量より角度が小さい.
	return angle < m_TurnSpeed;
}
