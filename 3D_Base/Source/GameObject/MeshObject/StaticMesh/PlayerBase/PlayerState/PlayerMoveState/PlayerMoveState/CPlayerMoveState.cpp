#include "CPlayerMoveState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"

CPlayerMoveState::CPlayerMoveState(CPlayerBase& pPlayer, float x, float z)
	: CPlayerState			( pPlayer )

	, m_InputDir			( x, 0.f, z )
	
	, m_CurrentSpeed		()
	, m_MoveSpeed			( 0.14f )
	, m_RotatingMoveSpeed	( 0.1f )
	, m_HitingMoveSpeed		( 0.03f )

{
}

CPlayerMoveState::~CPlayerMoveState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerMoveState::Enter()
{
}

//--- 状態の終了時に呼び出す ---.
void CPlayerMoveState::Exit()
{
	//歩いていない.
	m_pPlayer.SetMoving(false);
}

//--- この状態の間に呼び出す ---.
void CPlayerMoveState::Update()
{
	//入力されていなかった場合.
	if (m_InputDir.x == 0 && m_InputDir.z == 0)
	{
		//移動していない状態へ.
		m_pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>(m_pPlayer));
		return;
	}

	//入力方向を3D空間と同じように設定.
	D3DXVECTOR3 dir(m_InputDir.x, 0, m_InputDir.z);

	//長さの二乗が誤差しきい値（0.001の二乗）より大きい場合.
	if (D3DXVec3LengthSq(&dir) > 0.000001f)
	{
		//正規化.
		D3DXVec3Normalize(&dir, &dir);
	}
	//異常値の場合.
	else
	{
		//移動していない状態へ.
		m_pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>(m_pPlayer));
		return;
	}

	//歩いている.
	m_pPlayer.SetMoving(true);

	CPlayerBase::HitPlayer hitPlayer = m_pPlayer.GetHitPlayer();

	if (hitPlayer.isHit)
	{
		float dot = D3DXVec3Dot(&m_InputDir, &hitPlayer.otherDir);
		dot = std::clamp(dot, -1.f, 1.f);

		if (dot > 0.f)
		{
			m_InputDir -= dir * dot;
			D3DXVec3Normalize(&dir, &m_InputDir);
		}
	}

	//現在の速度を取得.
	m_CurrentSpeed = GetMoveSpeed();

	//ベクトル量の計算.
	D3DXVECTOR3 velocity = dir * m_CurrentSpeed;
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	//ベクトル量を足す.
	playerPos += velocity;

	//プレイヤーの位置の設定.
	m_pPlayer.SetPosition(playerPos);
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- 現在の速度にふさわしい数値を渡す ---.
float CPlayerMoveState::GetMoveSpeed()
{
	//回転している場合.
	if (m_pPlayer.IsTurning())
	{
		return m_RotatingMoveSpeed;
	}
	//動作不可能（位置の微調整は可能）の場合.
	if (m_pPlayer.IsAnyActionState<CPlayerPushedState, CPlayerFallingState>())
	{
		return m_HitingMoveSpeed;
	}

	return m_MoveSpeed;	//通常速度.
}
