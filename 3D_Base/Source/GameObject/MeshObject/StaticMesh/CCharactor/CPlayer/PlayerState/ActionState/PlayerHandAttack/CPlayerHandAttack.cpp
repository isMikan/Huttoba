#include "CPlayerHandAttack.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"

#include "TimeManager/CTimeManager.h"

CPlayerHandAttack::CPlayerHandAttack()
	: m_RightOffset		( 0.f, 0.f, 0.f )
	, m_LeftOffset		( 0.f, 0.f, 0.f )
	, m_RightPos		( 0.f, 0.f, 0.f )
	, m_LeftPos			( 0.f, 0.f, 0.f )

	, m_CenterOffset	( 0.2f )

	, m_StartTime		()
	, m_EndTime			( 0.1f )

	, m_CurrentSpeed	()
	, m_AttackSpeed		( 7.f )
	, m_MovingAtkSpeed	( 18.f )
{
}

CPlayerHandAttack::~CPlayerHandAttack()
{
}

void CPlayerHandAttack::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//手の位置を調整するための数値を取得.
	m_RightOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	m_LeftOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//デフォルトの調整位置よりも中心寄りにする.
	m_RightOffset.x -= m_CenterOffset;
	m_LeftOffset.x += m_CenterOffset;

	//位置計算.
	m_RightPos = pPlayer.HandPositionMath(m_RightOffset);
	m_LeftPos = pPlayer.HandPositionMath(m_LeftOffset);
}

void CPlayerHandAttack::Exit(CPlayer& pPlayer)
{
	//手の位置を調整するための数値を取得.
	m_RightOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	m_LeftOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//位置計算.
	m_RightPos = pPlayer.HandPositionMath(m_RightOffset);
	m_LeftPos = pPlayer.HandPositionMath(m_LeftOffset);

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(m_RightPos);
	pPlayer.GetPlayerLeftHand().SetPosition(m_LeftPos);
}

void CPlayerHandAttack::Update(CPlayer& pPlayer)
{
	float totalTime = CTimeManager::GetInstance()->GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (totalTime - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerActionIdle>());
		return;
	}

	m_CurrentSpeed = m_AttackSpeed;
	if (pPlayer.IsMoving())
	{
		m_CurrentSpeed = m_MovingAtkSpeed;
	}

	//前回のフレームからの経過時間.
	float deltaTime = CTimeManager::GetInstance()->GetDeltaTime();

	//プレイヤーの位置.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	//正面の方向を取得.
	D3DXVECTOR3 forward = pPlayer.GetForward();
	D3DXVec3Normalize(&forward, &forward);	//正規化.

	//プレイヤーの正面方向に手を押し出す.
	m_RightPos += forward * m_CurrentSpeed * deltaTime;
	m_LeftPos += forward  * m_CurrentSpeed * deltaTime;

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(m_RightPos);
	pPlayer.GetPlayerLeftHand().SetPosition(m_LeftPos);
}

void CPlayerHandAttack::Handle(CPlayer& pPlayer, int inputKey)
{
}
