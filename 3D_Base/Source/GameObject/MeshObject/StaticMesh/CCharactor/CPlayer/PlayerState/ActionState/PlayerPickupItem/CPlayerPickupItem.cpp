#include "CPlayerPickupItem.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"

#include "TimeManager/CTimeManager.h"

CPlayerPickupItem::CPlayerPickupItem()
	: m_RightHandPos		( 0.f, 0.f, 0.f )
	, m_LeftHandPos			( 0.f, 0.f, 0.f )

	, m_CenterOffset		( 0.2f )
	
	, m_StartTime			()
	, m_EndTime				( 0.2f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian(30.f) )
	, m_PhaseSplit			( 0.5f )

	, m_CurrentForwardHandSpeed		( 0.f )
	, m_StoppingForwardHandSpeed	( 2.5f )
	, m_MovingForwardHandSpeed		( 10.5f )
	, m_DownHandSpeed				( -1.5f )

	, m_StartQuat					()
{
}

CPlayerPickupItem::~CPlayerPickupItem()
{
}

void CPlayerPickupItem::Enter(CPlayer& pPlayer)
{
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayer.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

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

		//手の最終位置を設定.
		if (i == 0)
		{
			m_RightHandPos = handPos;
		}
		else
		{
			m_LeftHandPos = handPos;
		}
	}
}

void CPlayerPickupItem::Exit(CPlayer& pPlayer)
{
	pPlayer.SetQuaternion(m_StartQuat);
}

void CPlayerPickupItem::Update(CPlayer& pPlayer)
{
	float totalTime = CTimeManager::GetInstance()->GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (totalTime - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerActionIdle>());
		return;
	}

	//前回のフレームからの経過時間.
	float deltaTime = CTimeManager::GetInstance()->GetDeltaTime();

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

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

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetQuaternion(pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	//前に進む手の速度を通常に設定.
	m_CurrentForwardHandSpeed = m_StoppingForwardHandSpeed;
	//移動していたら早くする.
	if (pPlayer.IsMoving())
	{
		m_CurrentForwardHandSpeed = m_MovingForwardHandSpeed;
	}

	//プレイヤーの正面方向に手を押し出す.
	m_RightHandPos += axes.forward * m_CurrentForwardHandSpeed * deltaTime;
	m_LeftHandPos += axes.forward * m_CurrentForwardHandSpeed * deltaTime;
	//プレイヤーの下方向に手を押し出す.
	m_RightHandPos += axes.up * m_DownHandSpeed * deltaTime;
	m_LeftHandPos += axes.up * m_DownHandSpeed * deltaTime;

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(m_RightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(m_LeftHandPos);
}