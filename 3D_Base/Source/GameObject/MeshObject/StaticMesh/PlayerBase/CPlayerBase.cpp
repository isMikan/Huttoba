#include "CPlayerBase.h"

#include "PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerPickupState/CPlayerPickupState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

#include "Item/ItemBase.h"	


CPlayerBase::CPlayerBase( int index )
	: m_PlayerID		( index )
		
	, m_pHead			( std::make_unique<CPlayerHead>() )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdleState>( *this, 0.f, 0.f ) )
	, m_pTurnState		( std::make_unique<CPlayerTurnIdleState>( *this, 0.f, 0.f ) )
	, m_pActionState	( std::make_unique<CPlayerActionIdleState>( *this ) )

	, m_IsMoving		( false )
	, m_IsTurning		( false )
	, m_IsHoldingItem	( false )

	, m_HitInfo			()

	, m_Bus				( GetBus() )
{
}

CPlayerBase::~CPlayerBase()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CPlayerBase::Update()
{
	//頭の調整位置を取得.
	D3DXVECTOR3 headOffsetPos = GetPlayerHead().GetOffsetPos();
	//頭の位置を設定.
	GetPlayerHead().SetPosition(GetObjectPos(headOffsetPos));

	//押された場合の処理.
	if (m_HitInfo.hitEvent == HitEvent::Pushed)
	{
		SetActionState(std::make_unique<CPlayerPushedState>(*this));
	}
	//吹き飛ばされた場合の処理.
	if (m_HitInfo.hitEvent == HitEvent::Knockback
		|| m_HitInfo.hitEvent == HitEvent::Knockdown)
	{
		SetActionState(std::make_unique<CPlayerKnockbackState>(*this));
	}

	//移動の状態を更新.
	m_pMoveState->Update();
	//回転の状態を更新.
	m_pTurnState->Update();
	//行動の状態を更新.
	m_pActionState->Update();

	CStaticMeshObject::Update();
}

//--- 描画処理 ---.
void CPlayerBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

void CPlayerBase::CreateCollider()
{
	//新しい CollisionDataFactory を使ったコリジョンデータの生成と登録.
	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::BSphere);

	m_pCollision =
		CollisionDataFactory::CreateSphereForMesh(
			this,	//当たり判定の主.
			mesh,	//当たり判定用メッシュ.
			CollisionBase::ColliderTag::Player	//主のタグ.
		);

}

//--- 移動状態を設定 ---.
void CPlayerBase::SetMoveState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pMoveState, std::move(newState));
}

//--- 回転状態を設定 ---.
void CPlayerBase::SetTurnState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pTurnState, std::move(newState));
}

//--- 行動状態を設定 ---.
void CPlayerBase::SetActionState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pActionState, std::move(newState));
	m_Bus.Publish(m_pActionState.get());
	
	//std::cout << typeid(*m_pActionState).name() << std::endl;
}

//--- 位置を設定するために計算 ---.
D3DXVECTOR3 CPlayerBase::GetObjectPos(D3DXVECTOR3 offset)
{
	//ローカル軸を取得.
	LocalAxes axes = GetLocalAxes();

	//方向による位置を調整.
	offset =
		axes.right * offset.x +
		axes.up * offset.y +
		axes.forward * offset.z;

	//プレイヤーに合わせて調整.
	D3DXVECTOR3 objPos = m_vPosition + offset;

	return objPos;
}

//--- ローカル座標軸を取得 ---.
CPlayerBase::LocalAxes CPlayerBase::GetLocalAxes()
{
	//ローカル軸.
	LocalAxes axes;

	//ローカル軸をクォータニオンにして計算.
	axes.right = RotateVectorByQuat(D3DXVECTOR3(1, 0, 0), m_vQuaternion);
	axes.up = RotateVectorByQuat(D3DXVECTOR3(0, 1, 0), m_vQuaternion);
	axes.forward = RotateVectorByQuat(D3DXVECTOR3(0, 0, 1), m_vQuaternion);
	//正規化.
	D3DXVec3Normalize(&axes.right, &axes.right);
	D3DXVec3Normalize(&axes.up, &axes.up);
	D3DXVec3Normalize(&axes.forward, &axes.forward);

	return axes;
}

//--- クォータニオンによるベクトル回転 ---.
D3DXVECTOR3 CPlayerBase::RotateVectorByQuat(
	D3DXVECTOR3 vector, D3DXQUATERNION quat)
{
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//ベクトルをクォータニオンに変換.
	D3DXQUATERNION vecQuat(vector.x, vector.y, vector.z, 0.f);

	//逆元.
	D3DXQUATERNION inverseRot;
	//逆元を求める(共役(-x,-y-,z,w)をベクトルサイズの二乗で割る).
	D3DXQuaternionInverse(&inverseRot, &quat);

	//途中結果(v * -q).
	D3DXQUATERNION temp;
	//ベクトルと逆元をかける(回転が歪まないように).
	D3DXQuaternionMultiply(&temp, &inverseRot, &vecQuat);
	//最終結果(q * {v * -q}).
	D3DXQUATERNION result;
	D3DXQuaternionMultiply(&result, &temp, &quat);

	//ベクトル分だけ返す.
	return D3DXVECTOR3(result.x, result.y, result.z);
}

//--- プレイヤーの初期角度から傾きを計算する ---.
D3DXQUATERNION CPlayerBase::TiltedQuat(
	D3DXQUATERNION baseQuat, D3DXVECTOR3 localAxes, float tiltAngle)
{
	//傾き用クォータニオン.
	D3DXQUATERNION tilt;
	//マトリクスをクォータニオンに変換.
	D3DXQuaternionRotationAxis(&tilt, &localAxes, tiltAngle);
	D3DXQUATERNION quat;
	D3DXQuaternionMultiply(&quat, &baseQuat, &tilt); //基準の姿勢に傾きを掛ける.
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	return quat;
}

//--- 押された時の移動量を計算する ---.
D3DXVECTOR3 CPlayerBase::Pushed(D3DXVECTOR3 sourcePos)
{
	//押されるベクトル.
	D3DXVECTOR3 dir = m_vPosition - sourcePos;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);
	//方向へ吹き飛び量分の位置へ.
	D3DXVECTOR3 pos = dir * m_PushForce;

	return pos;
}

//--- 攻撃を受けた時のの移動量 ---.
D3DXVECTOR3 CPlayerBase::GetVelocity(
	D3DXVECTOR3 sourcePos, float speed, float angle)
{
	m_HitForce = speed;	//強さを設定.

	//飛ぶベクトル.
	D3DXVECTOR3 dir = m_vPosition - sourcePos;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);

	//角度60度上方向.
	angle = D3DXToRadian(60.f);

	D3DXVECTOR3 velocity{};
	velocity.x = cos(angle) * speed * dir.x;	//x軸方向に.
	velocity.z = cos(angle) * speed * dir.z;	//z軸方向に.
	velocity.y = sin(angle) * speed;

	return velocity;
}

//--- 角度を0～360度にする ---.
float CPlayerBase::WrapAngle(float value)
{
	//360度.
	const float twoPi = 2.f * D3DX_PI;

	//360度での余り.
	value = fmodf(value, twoPi);
	//余りが 0 未満なら足す.
	if (value < 0.f) value += twoPi;

	return value;
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- 状態遷移の処理関数 ---.
void CPlayerBase::ChangeState(
	std::unique_ptr<CPlayerState>& currentState,
	std::unique_ptr<CPlayerState> newState)
{
	if (currentState != nullptr)
	{
		//状態の終了処理.
		currentState->Exit();
	}

	//新しい状態にする.
	currentState = std::move(newState);

	if (currentState != nullptr)
	{
		//状態の開始処理.
		currentState->Enter();
	}
}

void CPlayerBase::OnCollision(CollisionBase* pOtherCollider)
{
	//	//攻撃を受けるプレイヤー.
	//	for (int hNo = 0;hNo < Player_Max;hNo++)
	//	{
	//		//攻撃するプレイヤー.
	//		for (int aNo = 0;aNo < Player_Max;aNo++)
	//		{
	//			if (hNo == aNo) continue;
	//			{
	//				switch (hNo)
	//				{
	//				case 2:
	//					m_pPlayers[hNo]->SetHitInfo(
	//						m_pPlayers[aNo]->GetPosition(), true, CPlayerBase::HitEvent::Pushed);
	//					break;
	//				case 3:
	//					break;
	//				default:
	//					m_pPlayers[hNo]->SetHitInfo(
	//						m_pPlayers[aNo]->GetPosition(), m_pPlayers[aNo]->GetPosition(), 10.f, true, CPlayerBase::HitEvent::Knockdown);
	//					break;
	//				}
	//
	//				m_pPlayers[aNo]->SetHitInfo(
	//					m_pPlayers[aNo]->GetPosition(), true, CPlayerBase::HitEvent::None);
	//			}
	//		}
	//	}

		// 衝突相手のタグをチェックし、応答を切り替える
	switch (pOtherCollider->GetTag())
	{
	case CollisionBase::ColliderTag::Player:

		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(pOtherCollider->GetListener()))
		{
			if (player->IsAnyActionState<CPlayerHandAttackState>())
			{
				SetHitInfo(
					GetVelocity(player->GetPosition(), 10.f,60.f), CPlayerBase::HitEvent::Knockdown);
			}
		}
		break;

	case CollisionBase::ColliderTag::Bomb:

		if (ItemBase* item = dynamic_cast<ItemBase*>(pOtherCollider->GetListener()))
		{
			if (IsAnyActionState<CPlayerPickupState>())
			{
				item->SetPlayer(this);
				item->SetState(ItemBase::State::Have);
			}
		}
	}
}
