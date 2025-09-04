#include "CPlayerHandWhiff.h"

#include "CPlayer.h"

#include "CPlayerMoveIdle.h"
#include "CPlayerRotationIdle.h"
#include "CPlayerActionIdle.h"

#include "CGameTimer.h"

CPlayerHandWhiff::CPlayerHandWhiff()
	: m_StartTime			()
	, m_EndTime				( 0.3f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian(30.f) )
	, m_PhaseSplit			( 0.5f )

	, m_RightHandStartPos	( 0.f, 0.f, 0.f )
	, m_LeftHandStartPos	( 0.f, 0.f, 0.f )
	, m_RightHandEndPos		( 0.f, 0.f, 0.f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandWhiff::~CPlayerHandWhiff()
{
}

void CPlayerHandWhiff::Enter(CPlayer& pPlayer)
{
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
    m_StartQuat = pPlayer.GetRotationQuat();

	//攻撃の開始時間を取得.
	m_StartTime = CGameTimer::GetInstance()->GetTotalTime();

	//手の開始位置を設定.
	m_RightHandStartPos = pPlayer.GetPlayerRightHand().GetPosition();
	m_LeftHandStartPos = pPlayer.GetPlayerLeftHand().GetPosition();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の調整リスト.
	D3DXVECTOR3 offset[]
	{
		rightOffset,
		leftOffset
	};
	//リストの最大数.
	int offsetMax = sizeof(offset) / sizeof(offset[0]);

	for (int i = 0;i < offsetMax; i++)
	{
		//方向に合わせて位置を調整.
		offset[i] =
			axes.right * offset[i].x +
			axes.up * offset[i].y +
			axes.forward * offset[i].z;

		//手の位置.
		D3DXVECTOR3 handPos = playerPos + offset[i];

		//手の最終位置を設定.
		if (i == 0)
		{
			m_RightHandEndPos = handPos;
		}
		else
		{
			m_LeftHandEndPos = handPos;
		}
	}
}

void CPlayerHandWhiff::Exit(CPlayer& pPlayer)
{
    pPlayer.SetRotationQuat(m_StartQuat);
}

void CPlayerHandWhiff::Update(CPlayer& pPlayer)
{
	pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdle>());
	pPlayer.SetRotationState(std::make_unique<CPlayerRotationIdle>());

	float totalTime = CGameTimer::GetInstance()->GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (totalTime - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerActionIdle>());
		return;
	}
	
	//全体の時間の現在の割合.
	float progress = (totalTime - m_StartTime) / m_EndTime;

	//時間の割合が半分より前なら(倒れる動き).
	if (progress < m_PhaseSplit) 
	{
		//倒れきるまでの現在の傾き割合.
		float ratio = progress / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * 割合.
		m_CurrentTiltAngle = m_TiltAngleMax * ratio;
	}
	//時間の割合が半分以上(戻る動き).
	else if (progress <= 1.0f)
	{
		//傾きの変わり目(m_PhaseSplit)からどれだけ経過したかを割って割合.
		float ratio = (progress - m_PhaseSplit) / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * (1 - 割合).
		m_CurrentTiltAngle = m_TiltAngleMax * (1.f - ratio);
	}
	else 
	{
		//終了後は0度.
		m_CurrentTiltAngle = 0.f;
	}

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetRotationQuat(pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));
	
	//D3DXVECTOR3 offset = D3DXVECTOR3(0.f, 0.f, 5.f);
	//pPlayer.SetPivotOffset(offset);

	float eased = sinf(progress * D3DX_PI * 0.5f);	//0.5かけて半円分の移動を計算.	

	D3DXVECTOR3 rightHandPos;
	D3DXVec3Lerp(&rightHandPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandPos;
	D3DXVec3Lerp(&leftHandPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);
}

void CPlayerHandWhiff::Handle(CPlayer& pPlayer, int inputKey)
{

}

std::string CPlayerHandWhiff::GetStateName() const
{
	return "HandWhiff";
}