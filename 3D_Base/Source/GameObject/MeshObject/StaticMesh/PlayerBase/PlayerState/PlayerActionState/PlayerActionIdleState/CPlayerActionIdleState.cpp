#include "CPlayerActionIdleState.h"

#include "PlayerBase/CPlayerBase.h"

CPlayerActionIdleState::CPlayerActionIdleState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )

	, m_StartTime			()
	, m_EndTime				( 2.f )
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
	//モーション開始時間.
	m_StartTime = CTimeManager::GetTotalTime();
		
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

	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//終了時間を過ぎた場合.
	if (t - m_StartTime > m_EndTime)
	{
		//モーション開始時間.
		m_StartTime = CTimeManager::GetTotalTime();
	}

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = progress = std::clamp(progress, 0.f, 1.f);

	float eased = cosf(progress * D3DX_PI * 2.f) * 0.08f;	//1周分の補正を計算.	

	//調整位置に足す.
	rightHandOffsetPos.y += eased;
	leftHandOffsetPos.y += eased;

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}
