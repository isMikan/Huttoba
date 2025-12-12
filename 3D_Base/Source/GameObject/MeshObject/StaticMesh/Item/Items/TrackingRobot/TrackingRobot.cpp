#include "TrackingRobot.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

#include "ChaseSensor/ChaseSensor.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<TrackingRobot>(ItemID::TrackingRobot); }

TrackingRobot::TrackingRobot()
	: m_pTarget			()
	, m_pChaseSensor	()

	, m_IsGround		( false )

	, m_Velocity		()
	, m_MoveSpeed		( 4.0f )	//値を変えると使用時の移動速度が変化
	, m_TurnRate		( 1.4f )	//値を変えると使用時の旋回の角度が変化

	, m_ThrowSpeed		( 6.0f )	//値を変えると投擲時の移動速度が変化

	, m_ExplosionTime	( 5.0f )	//値を変えると爆発するまでの時間が変化
	, m_ExplosionCnt	( 0.0f )

	, m_KnockBackPower	( 10.0f )	//値を変えるとプレイヤーの吹き飛ばし力が変化

	, m_ColorTimer		( 0.0 )

	, m_IsExploded		( false )

	, m_CollisionOffSet	(0.0f, 0.0f, /*1.3*/1.8f)
{
	Init();
	//m_vPosition = D3DXVECTOR3(4.0f, 0.0f, 0.0f);
}

TrackingRobot::~TrackingRobot()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void TrackingRobot::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::TrackingRobot));

	m_State = IItemObserver::IItemObserver::State::Spawn;

	m_tGravity = 0.01f;

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::TrackingRobot,
		AssetManager::Mesh(StaticMeshList::PickUpCol),
		this
	);
}

void TrackingRobot::Update()
{
	ItemBase::Update();
}

void TrackingRobot::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void TrackingRobot::Spawn()
{
	//y座標が一定以下になると状態を変化させているが、
	//地面とぶつかったときにOnGroundに変化させるようにする

	//落下処理
	if (m_vPosition.y > 0.5f)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += 0.001f;
	}
	else
	{
		//状態を地面についたときに変更
		m_State = IItemObserver::IItemObserver::State::OnGround;
	}
}

void TrackingRobot::OnGround()
{
}

void TrackingRobot::Have()
{
	HaveMove();
}

void TrackingRobot::Use()
{
	UseMove();
}

void TrackingRobot::Throw()
{
	ThrowMove();
}

void TrackingRobot::Destroy()
{
	DestroyItem();
}

void TrackingRobot::ItemState(IItemObserver::State state)
{
	switch (state)
	{
	case IItemObserver::IItemObserver::State::Use:
		OneEnterUse();
		break;
	case IItemObserver::IItemObserver::State::Throw:
		OneEnterThrow();
		break;
	default:
		break;
	}
}

void TrackingRobot::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_State == State::Use)
			{
				if (m_pPlayer != player)
					Explosion();
			}

			if (m_IsExploded)
			{
				Smash(*player);
			}

			if (m_State == IItemObserver::State::Throw && m_pPlayer != player)
			{
				Smash(*player);
			}
		}
	}
}

void TrackingRobot::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void TrackingRobot::UseMove()
{
	//if (m_pChaseSensor->GetIsHitGround())
	if(m_IsOnGround)
	{
		UpdateChaseSensor();

		if (m_pTarget != nullptr)
		{
			Homing(m_pTarget->GetPosition());
		}
		else
		{
			D3DXMATRIX matRot;

			//クォータニオンをマトリックス(行列)に変換
			D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);

			//行列の中にあるZ軸成分を取り出す
			D3DXVECTOR3 forward = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

			//取り出したZ軸成分をノーマライズ
			D3DXVec3Normalize(&forward, &forward);

			m_Velocity = forward * m_MoveSpeed;
		}


		m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
	}
	else
	{
		Explosion();
	}

	//m_pChaseSensor->SetIsHitGround(false);
}

void TrackingRobot::ThrowMove()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.3)
	{
		DestroyItem();
	}

	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void TrackingRobot::OneEnterUse()
{
	//プレイヤーのクォータニオン(向いている方向)記録
	m_vQuaternion = m_pPlayer->GetQuaternion();

	D3DXMATRIX matRot;

	//クォータニオンをマトリックス(行列)に変換
	D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);

	//行列の中にあるZ軸成分を取り出す
	D3DXVECTOR3 forward = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//取り出したZ軸成分をノーマライズ
	D3DXVec3Normalize(&forward, &forward);

	//投げた瞬間に別のアイテムを持ったり使ったりできるように追加
	m_pPlayer->SetHoldingItem(nullptr);

	//索敵判定クラスの生成
	m_pChaseSensor = std::make_unique<ChaseSensor>(m_vPosition, m_CollisionOffSet);

	//使用したプレイヤーを索敵から無視する
	m_pChaseSensor->SetIgnoredPlayer(m_pPlayer);
}

void TrackingRobot::OneEnterThrow()
{
	//プレイヤーのクォータニオン(向いている方向)記録
	m_vQuaternion = m_pPlayer->GetQuaternion();

	D3DXMATRIX matRot;

	//クォータニオンをマトリックス(行列)に変換
	D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);

	//行列の中にあるZ軸成分を取り出す
	D3DXVECTOR3 forward = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//取り出したZ軸成分をノーマライズ
	D3DXVec3Normalize(&forward, &forward);

	m_Velocity = forward * m_ThrowSpeed;
}

void TrackingRobot::Explosion()
{
	//爆発時に一度だけ処理する
	if (!m_IsExploded)
	{
		//爆発フラグをオンに
		m_IsExploded = true;

		static ::EsHandle hEffect = -1;

		hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

		//拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

		DestroyItem();
	}
}

void TrackingRobot::Smash(CPlayerBase& playiers)
{
	//爆弾とプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, CalculateForceScalar(len), 60.0f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);
}

void TrackingRobot::ChangeColor()
{
	m_ColorTimer += CTimeManager::GetDeltaTime();

	//点滅のスピードを経過時間/爆発するまでの時間をして割合で出す
	float speed = 10.0f * (static_cast<float>(m_ColorTimer / m_ExplosionTime));

	//+1.0fをすることで、sinの値が0~2の間の値になり、*0.5することで0~1の間の値が取れる
	float blinkRate = (sinf(static_cast<float>(m_ColorTimer) * speed) + 1.0f) * 0.5f;

	//灰色のカラーコード
	D3DXVECTOR4 gray = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//赤色のカラーコード
	D3DXVECTOR4 red = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//値が増加と減少がそれぞれあるので使いわけていく
	D3DXVECTOR4 color;

	D3DXVec4Lerp(&color, &gray, &red, blinkRate);

	m_ObjectColor[0].diffuse = color;
}

float TrackingRobot::CalculateForceScalar(float distance)
{
	//爆発の当たる範囲を仮設定
	//当たり判定用メッシュの大きさにしたい
	float maxDist = 2;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//爆発の最小吹き飛ばし力
	float minPower = 5.0f;

	//爆発の最大吹き飛ばし力
	float maxPower = m_KnockBackPower;

	float power = minPower + (maxPower - minPower) * ratio;

	return power;
}

void TrackingRobot::Homing(D3DXVECTOR3 targetPos)
{
	//ターゲット方向へのベクトル
	D3DXVECTOR3 vec = targetPos - m_vPosition;

	//y軸方向は無視して計算する
	float dist = vec.x * vec.x + vec.z * vec.z;

	//距離が近すぎると計算しない
	if (dist < 0.001f) return;

	dist = sqrtf(dist);

	//目標への単位ベクトルに変換
	D3DXVECTOR3 nolVec = { vec.x / dist,0,vec.z / dist };

	//目標の向きを計算(モデルの正面の方向によってxとzが逆にすることも)
	float angle = std::atan2(nolVec.x, nolVec.z);

	D3DXQUATERNION targetRot;
	//y軸を回転の軸に
	D3DXVECTOR3 axisY = { 0.0f,1.0f,0.0f };

	//2引数で指定した回転の軸に３引数で指定した角度のクォータニオンを返す
	D3DXQuaternionRotationAxis(&targetRot, &axisY, angle);

	//旋回率
	float turnRate = m_TurnRate * CTimeManager::GetDeltaTime();
	
	turnRate = std::clamp(turnRate, 0.f, 1.f);

	D3DXQuaternionSlerp(&m_vQuaternion, &m_vQuaternion, &targetRot, turnRate);

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);
	D3DXVECTOR3 forward(matRot._31, 0.0f, matRot._33); //Y成分は0にしておく
	D3DXVec3Normalize(&forward, &forward);

	//移動速度を反映
	m_Velocity.x = forward.x * m_MoveSpeed;
	m_Velocity.z = forward.z * m_MoveSpeed;
}

void TrackingRobot::UpdateChaseSensor()
{
	m_pChaseSensor->SetPosition(m_vPosition);
	m_pChaseSensor->SetQuaternion(m_vQuaternion);

	//近いプレイヤーを計算
	m_pChaseSensor->Update();

	//一番近いプレイヤーのポインタが入る
	m_pTarget = m_pChaseSensor->GetTarget();
}
