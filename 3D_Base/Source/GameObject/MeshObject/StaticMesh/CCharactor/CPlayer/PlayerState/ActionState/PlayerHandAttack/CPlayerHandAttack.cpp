#include "CPlayerHandAttack.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/PlayerHandWhiff/CPlayerHandWhiff.h"

#include "TimeManager/CTimeManager.h"

CPlayerHandAttack::CPlayerHandAttack()
	: m_RightHandPos	( 0.f, 0.f, 0.f )
	, m_LeftHandPos		( 0.f, 0.f, 0.f )

	, m_CenterOffset	( 0.2f )

	, m_StartTime		()
	, m_EndTime			( 0.07f )

	, m_CurrentSpeed	()
	, m_AttackSpeed		( 8.f )
	, m_MovingAtkSpeed	( 18.f )

	, m_StartQuat		( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandAttack::~CPlayerHandAttack()
{
}

void CPlayerHandAttack::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	m_StartQuat = pPlayer.GetRotationQuat();

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//元の調整位置よりも中心寄りにする.
	rightOffset.x -= m_CenterOffset;
	leftOffset.x += m_CenterOffset;

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

		if (i == 0)
		{
			m_RightHandPos = handPos;
			//右手の位置を設定.
			pPlayer.GetPlayerRightHand().SetPosition(handPos);
		}
		else
		{
			m_LeftHandPos = handPos;
			//左手の位置を設定.
			pPlayer.GetPlayerLeftHand().SetPosition(handPos);
		}
	}
}

void CPlayerHandAttack::Exit(CPlayer& pPlayer)
{
}

void CPlayerHandAttack::Update(CPlayer& pPlayer)
{
	float totalTime = CTimeManager::GetInstance()->GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (totalTime - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerHandWhiff>());
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
	D3DXVECTOR3 forward = pPlayer.GetLocalAxes().forward;
	D3DXVec3Normalize(&forward, &forward);	//正規化.

	//プレイヤーの正面方向に手を押し出す.
	m_RightHandPos += forward * m_CurrentSpeed * deltaTime;
	m_LeftHandPos += forward  * m_CurrentSpeed * deltaTime;

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(m_RightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(m_LeftHandPos);
}
