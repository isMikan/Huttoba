#include "CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

CPlayerActionIdleState::CPlayerActionIdleState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
{
}

CPlayerActionIdleState::~CPlayerActionIdleState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerActionIdleState::Enter()
{
	//クォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの設定.
	m_pPlayer.SetQuaternion(
		0.f, quat.y, 0.f, quat.w);
}

//--- 状態の終了時に呼び出す ---.
void CPlayerActionIdleState::Exit()
{
	//クォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの設定.
	m_pPlayer.SetQuaternion(quat);
}

//--- この状態の間に呼び出す ---.
void CPlayerActionIdleState::Update()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffsetPos = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}
