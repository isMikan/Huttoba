#include "CPlayerKnockdownState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/Player/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"

#include "Input/CInputManager.h"

CPlayerKnockdownState::CPlayerKnockdownState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 0.7f )	//終了させる割合.

	, m_DecreaseTriggerTime	()
	, m_TimeDecrease		( 0.2f )

	, m_ShakeSpeed			( 3.f )
	, m_ShakeWidth			( 1.2f )

	, m_PrevSthikX			( 0.1f )	//0 にすると積が変わらないので.
	, m_PrevSthikY			( 0.1f )	//0 にすると積が変わらないので.

	, m_IsTimeDecreasing	( false )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )

	, m_RightHandPos		( -0.1f, 0.3f, 0.1f )
	, m_LeftHandPos			( 0.1f, 0.3f, 0.1f )
{
}

CPlayerKnockdownState::~CPlayerKnockdownState()
{
}

void CPlayerKnockdownState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlayLoop(enSoundList::SE_Knockdown);

	m_pPlayer.SetPlayerEvent(CPlayerBase::PlayerEvent::Knockdown);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();
	m_pPlayer.SetQuaternion(m_StartQuat);

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//手の開始位置を設定.
	m_RightHandPos += rightHandOffset;
	m_LeftHandPos += leftHandOffset;

	//吹き飛ばされ量から終了する時間を計算.
	m_EndTime = m_pPlayer.GetHitInfo().force * m_EndTime;
}

void CPlayerKnockdownState::Exit()
{
	//SEを止める.
	AssetManager::Sound()->Stop(enSoundList::SE_Knockdown);
}

void CPlayerKnockdownState::Update()
{
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerGetUpState>(m_pPlayer));
		return;
	}

	//プレイヤークラスの場合.
	if(dynamic_cast<CPlayer*>(&m_pPlayer))
	{
		ChildPlayer(m_pPlayer.GetPlayerID());
	}

	//フラグが ture の間減らす.
	if(m_IsTimeDecreasing)
	{
		TimeDecrease();
	}

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(m_RightHandPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(m_LeftHandPos));
}

void CPlayerKnockdownState::ChildPlayer(int index)
{
	float x = CInputManager::GetLeftSthikX(index);
	float y = CInputManager::GetLeftSthikY(index);

	if (IsInput(x, y, index))
	{
		if (!m_IsTimeDecreasing)
		{
			std::cout << "レバガチャ入力検知！" << std::endl;

			//レバガチャ成功時の値を保存.
			m_PrevSthikX = x;
			m_PrevSthikY = y;

			m_EndTime -= 0.5f;
			m_DecreaseTriggerTime = static_cast<float>(CTimeManager::GetTotalTime());
			m_IsTimeDecreasing = true;
		}
	}
}

void CPlayerKnockdownState::TimeDecrease()
{
	//経過時間を取得.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//終了時間が過ぎたら終わる.
	if (t - m_DecreaseTriggerTime > m_TimeDecrease)
	{
		std::cout << "レバガチャ終了！" << std::endl;
		m_IsTimeDecreasing = false;
		return;
	}

	//全体の時間の現在の割合.
	float progress = (t - m_DecreaseTriggerTime) / m_TimeDecrease;
	progress = std::clamp(progress, 0.f, 1.f);

	//揺れる動作を計算.
	float offset = cosf(progress * D3DX_PI * m_ShakeSpeed) * m_ShakeWidth;

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	//横軸に揺らす.
	playerPos += axes.right * offset * dt;

	m_pPlayer.SetPosition(playerPos);

}

bool CPlayerKnockdownState::IsInput(float x, float y, int index) const
{
	//コントローラが接続されている場合.
	if (CInputManager::IsConnect(index))
	{
		//積がマイナスになったら、以前と現在のスティック数値の符号が異なる.
		if (m_PrevSthikX * x < 0.f 
			|| m_PrevSthikY * y < 0.f)
		{
			return true;
		}
	}
	else if (CInputManager::IsDown(Action::MoveUp, index)
		|| CInputManager::IsDown(Action::MoveDown, index)
		|| CInputManager::IsDown(Action::MoveLeft, index)
		|| CInputManager::IsDown(Action::MoveRight, index))
	{
		return true;
	}

	return false;
}
