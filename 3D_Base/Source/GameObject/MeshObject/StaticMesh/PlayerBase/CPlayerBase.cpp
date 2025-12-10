#include "CPlayerBase.h"

#include "Item/ItemBase.h"	
#include "Item/Items/Boomerang/Boomerang.h"
#include "Collision/CollisionUtility/CollisionUtility.h"
#include "Scene/SceneData/CSceneData.h"

CPlayerBase::CPlayerBase( int index )
	: m_PlayerID		( index )
		
	, m_pHead			( std::make_unique<CPlayerHead>() )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdleState>( *this ) )
	, m_pTurnState		( std::make_unique<CPlayerTurnIdleState>( *this ) )
	, m_pActionState	( std::make_unique<CPlayerActionIdleState>( *this ) )

	, m_pHoldingItem	( nullptr )

	, m_Control			( ActionInstruct::None )
	, m_HitAttack		()
	, m_HitPlayer		()
	, m_KnockdownTime	()

	, m_IsMoving		( false )
	, m_IsTurning		( false )
	, m_IsOnGround		( false )

	, m_HitForce		()

	, m_Bus				( GetBus() )
{
}

CPlayerBase::~CPlayerBase()
{
	m_pHoldingItem = nullptr;
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CPlayerBase::Update()
{
	CStaticMeshObject::Update();
	
	//頭の調整位置を取得.
	D3DXVECTOR3 headOffsetPos = GetPlayerHead().GetOffsetPos();
	//頭の位置を設定.
	GetPlayerHead().SetPosition(GetObjectPos(headOffsetPos));

	//地面についておらず、落ちる状態じゃない場合.
	if (!m_IsOnGround
		&& !IsAnyActionState<
		CPlayerFallingState, 
		CPlayerKnockbackState>())
	{
		m_HitAttack.hitEvent = HitEvent::None;
		SetActionState(std::make_unique<CPlayerFallingState>(*this));
	}

	//押された場合の処理.
	if (m_HitAttack.hitEvent == HitEvent::Pushback)
	{
		SetActionState(std::make_unique<CPlayerPushedState>(*this));
	}
	//吹き飛ばされた場合の処理.
	if (m_HitAttack.hitEvent == HitEvent::Knockback
		|| m_HitAttack.hitEvent == HitEvent::Knockdown)
	{
		SetActionState(std::make_unique<CPlayerKnockbackState>(*this));
	}

	//アイテムが存在する場合.
	if (m_pHoldingItem)
	{
		//アイテムを投げる.
		if (m_Control == ActionInstruct::ToggleItem)
		{
			if(auto boomerang = dynamic_cast<Boomerang*>(m_pHoldingItem); 
				(boomerang && !boomerang->GetIsUseThrow()) 
				|| (!boomerang))
			{
				m_pHoldingItem->SetState(IItemObserver::State::Throw);
				SetActionState(std::make_unique<CPlayerThrowState>(*this));
			}
		}
		//アイテムの攻撃.
		if (m_Control == ActionInstruct::Attack)
		{
			if (!m_pHoldingItem
				|| (dynamic_cast<Boomerang*>(m_pHoldingItem)
				&& dynamic_cast<Boomerang*>(m_pHoldingItem)->GetIsUseThrow()))
			{
				SetActionState(std::make_unique<CPlayerHandAttackState>(*this));
			}
			else
			{
				m_pHoldingItem->SetState(IItemObserver::State::Use);
				SetActionState(std::make_unique<CPlayerItemAttackState>(*this));
			}
		}
	}
	else
	{
		//手の攻撃.
		if (m_Control == ActionInstruct::Attack)
		{
			SetActionState(std::make_unique<CPlayerHandAttackState>(*this));
		}
	}

	//移動の状態を更新.
	m_pMoveState->Update();
	//回転の状態を更新.
	m_pTurnState->Update();
	//行動の状態を更新.
	m_pActionState->Update();

	m_HitPlayer.isHit = false;
}

//--- 描画処理 ---.
void CPlayerBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

//--- リザルト用更新処理 ---.
void CPlayerBase::ResultUpdate()
{
	//頭の調整位置を取得.
	D3DXVECTOR3 headOffsetPos = GetPlayerHead().GetOffsetPos();
	//頭の位置を設定.
	GetPlayerHead().SetPosition(GetObjectPos(headOffsetPos));

	//地面についておらず、落ちる状態じゃない場合.
	if (!m_IsOnGround
		&& !IsAnyActionState<
		CPlayerFallingState,
		CPlayerKnockbackState>())
	{
		SetActionState(std::make_unique<CPlayerFallingState>(*this));
	}

	//アイドル状態の場合.
	if(IsAnyActionState<CPlayerActionIdleState>())
	{
		if (CSceneData::GetPlayerLiving(m_PlayerID))
		{
			SetActionState(std::make_unique<CPlayerResultWin_TypeA>(*this));
		}
		else
		{
			SetActionState(std::make_unique<CPlayerResultLose_TypeA>(*this));
		}
	}

	//行動の状態を更新.
	m_pActionState->Update();
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
}

//--- 当たり判定生成 ---.
void CPlayerBase::CreateCollider()
{
	//新しい CollisionDataFactory を使ったコリジョンデータの生成と登録.
	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::BCapsule);

	m_pCollision =
		CollisionDataFactory::CreateCapsuleForMesh(
			CollisionBase::ColliderTag::Player,
			mesh,	
			this
		);

	////新しい CollisionDataFactory を使ったコリジョンデータの生成と登録.
	//std::shared_ptr<CStaticMesh> hand = AssetManager::Mesh(StaticMeshList::BCapsule);

	//m_pCollision =
	//	CollisionDataFactory::CreateCapsuleForMesh(
	//		CollisionBase::ColliderTag::HandAttack,
	//		hand,	
	//		this
	//	);
}

//--- 地面との衝突判定 ---.
void CPlayerBase::OnGroundCollision(CGroundManager& pGroundMgr)
{
	//関数のoutで当たったy座標をもらう.
	float groundY = 0.0f;

	//CollisionManagerに判定を依頼し、結果をそのまま返す.
	m_IsOnGround = CollisionUtility::CheckGroundContact(
		GetPosition(),
		pGroundMgr,
		groundY
	);

	m_IsAboveGround = m_IsOnGround;
	if (m_vPosition.y > 0.3f)
	{
		m_IsOnGround = false;
	}
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

//--- プレイヤーの初期角度から傾きを計算 ---.
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

//--- 押された時の移動量を計算 ---.
D3DXVECTOR3 CPlayerBase::GetPushbackVelocity(D3DXVECTOR3 sourcePos, float power)
{
	//押されるベクトル.
	D3DXVECTOR3 dir = m_vPosition - sourcePos;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);
	//方向へ吹き飛び量分の位置へ.
	D3DXVECTOR3 pos = dir * power;
	pos.y = 0.f;

	return pos;
}

//--- 攻撃を受けた時のの移動量を計算 ---.
D3DXVECTOR3 CPlayerBase::GetKnockbackVelocity(
	D3DXVECTOR3 sourcePos, float power, float angle)
{
	m_HitForce = power;	//強さを設定.

	//飛ぶベクトル.
	D3DXVECTOR3 dir = m_vPosition - sourcePos;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);

	//角度60度上方向.
	angle = D3DXToRadian(angle);

	D3DXVECTOR3 velocity{};
	velocity.x = cos(angle) * power * dir.x;	//x軸方向に.
	velocity.z = cos(angle) * power * dir.z;	//z軸方向に.
	velocity.y = sin(angle) * power;
	
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
	if (currentState)
	{
		//状態の終了処理.
		currentState->Exit();
	}

	//新しい状態にする.
	currentState = std::move(newState);

	if (currentState)
	{
		//状態の開始処理.
		currentState->Enter();
	}
}

//--- 衝突判定 ---.
void CPlayerBase::OnCollision(CollisionBase* pOtherCollider)
{
	//衝突相手のタグをチェックし、応答を切り替える.
	switch (pOtherCollider->GetTag())
	{
	case CollisionBase::ColliderTag::Player:

		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(pOtherCollider->GetListener()))
		{
			if (player->IsAnyActionState<CPlayerHandAttackState>())
			{
				//相手側にヒットしていることを伝えるため.
				player->SetHitAttack(
					player->GetPosition(), CPlayerBase::HitEvent::HandAttack);

#if 0
				SetHitAttack(
					GetKnockbackVelocity(player->GetPosition(), 10.f, 60.f), CPlayerBase::HitEvent::Knockback);
#else
				SetHitAttack(
					GetPushbackVelocity(player->GetPosition(), 3.f), CPlayerBase::HitEvent::Pushback);
#endif
			}
			else
			{
				//相手の位置を取得.
				D3DXVECTOR3 hitPlayerPos = player->GetPosition();
				D3DXVECTOR3 dir = hitPlayerPos - m_vPosition;
				D3DXVec3Normalize(&dir, &dir);

				m_HitPlayer.otherDir = dir;
				m_HitPlayer.isHit = true;
			}
		}

		break;

	case CollisionBase::ColliderTag::Haetataki:
	case CollisionBase::ColliderTag::SmashBat:
	case CollisionBase::ColliderTag::Bomb:
	case CollisionBase::ColliderTag::Mushroom:
	case CollisionBase::ColliderTag::Fan:
	case CollisionBase::ColliderTag::Magnet:
	case CollisionBase::ColliderTag::TrackingRobot:

		if (ItemBase* item = dynamic_cast<ItemBase*>(pOtherCollider->GetListener()))
		{
			//アイテムが地面にある状態の場合.
			if(item->GetState() == IItemObserver::State::OnGround)
			{
				//拾う指示をされ、アイテムを持っていない場合.
				if (m_Control == ActionInstruct::ToggleItem
					&& !m_pHoldingItem)
				{
					item->SetPlayer(this);
					item->SetState(IItemObserver::State::Have);
					SetActionState(std::make_unique<CPlayerPickupState>(*this));
					m_pHoldingItem = item;
				}
			}
		}
		break;
	case CollisionBase::ColliderTag::Boomerang:

		if (Boomerang* item = dynamic_cast<Boomerang*>(pOtherCollider->GetListener()))
		{
			if (m_pHoldingItem == item)
			{
				if (item->GetIsComeBack())
				{
					item->SetState(IItemObserver::State::Have);
				}
			}
		}
		break;
	default:
		break;
	}
}
