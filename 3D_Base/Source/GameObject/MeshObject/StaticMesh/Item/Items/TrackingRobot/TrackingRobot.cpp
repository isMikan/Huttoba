#include "TrackingRobot.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<TrackingRobot>("TrackingRobot"); }

TrackingRobot::TrackingRobot()
	: m_pTargetList		()
	, m_pTarget			()

	, m_IsHoming		( false )

	, m_Velocity		()
	, m_MoveSpeed		(3.0f)	//値を変えると爆弾の移動相度が変化

	, m_IsThrow			(true)

	, m_ExplosionTime	(5.0f)	//値を変えると爆発するまでの時間が変化
	, m_ExplosionCnt	(0.0f)

	, m_KnockBackPower	(10.0f)	//値を変えるとプレイヤーの吹き飛ばし力が変化

	, m_ColorTimer		(0.0)

	, m_IsExploded		(false)
{
	Init();
}

TrackingRobot::~TrackingRobot()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void TrackingRobot::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::TrackingRobot));

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::ExplosionCol);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		this,
		mesh,
		CollisionBase::ColliderTag::Bomb
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
		m_State = ItemBase::State::OnGround;
	}
}

void TrackingRobot::OnGround()
{
	//後で地面がなければ落下する処理を追加する

}

void TrackingRobot::Have()
{
	HaveMove();
}

void TrackingRobot::Use()
{
	UseMove();
	ChangeColor();
}

void TrackingRobot::Throw()
{
	ThrowMove();
	ChangeColor();
}

void TrackingRobot::Destroy()
{
	m_IsDestroy = true;
}

void TrackingRobot::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayer* player = dynamic_cast<CPlayer*>(other->GetListener()))
		{
			if (m_IsExploded)
			{
				Smash(*player);
				return;
			}

			
			if (m_IsHoming)
			{
				//当たったプレイヤーを記憶
				//m_pTargetList.push_back(player);

				m_pTarget = player;
			}
		}
	}
}

void TrackingRobot::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
}

void TrackingRobot::UseMove()
{
	if (m_IsThrow)
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

		m_Velocity = forward * m_MoveSpeed;

		m_Velocity.y = 10.0f;

		m_IsThrow = false;
	}

	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	if (m_vPosition.y > 0.5f)
	{
		//m_vPosition.y -= m_tGravity;
		m_Velocity.y -= m_tGravity;
		m_tGravity += 0.001f;
	}
	else
	{
		m_Velocity.y = 0;
		m_IsHoming = true;

		//Explosion();
	}

	Homing();

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void TrackingRobot::ThrowMove()
{
	//使用の処理と投げるの処理が同じなのでUseMoveを使用
	UseMove();
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

		m_State = ItemBase::State::Destroy;
	}
}

void TrackingRobot::Smash(CPlayer& playiers)
{
	//爆弾とプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	float i = CalculateForceScalar(len);

	D3DXVECTOR3 SmashVel = playiers.GetVelocity(m_vPosition, CalculateForceScalar(len), 60.0f);

	playiers.SetHitInfo(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);
}

void TrackingRobot::ChangeColor()
{
	m_ColorTimer += CTimeManager::GetDeltaTime();

	//点滅のスピードを経過時間/爆発するまでの時間をして割合で出す
	float speed = 10.0f * (static_cast<float>(m_ColorTimer) / m_ExplosionTime);

	//+1.0fをすることで、sinの値が0~2の間の値になり、*0.5することで0~1の間の値が取れる
	float blinkRate = (sinf(static_cast<float>(m_ColorTimer) * speed) + 1.0) * 0.5;

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

void TrackingRobot::Homing()
{
	if (m_IsHoming&&m_pTarget!=nullptr)
	{
		//プレイヤー方向へのベクトル
		D3DXVECTOR3 vec = m_pTarget->GetPosition() - m_vPosition;

		float dist = std::sqrtf(vec.x * vec.x + vec.z * vec.z);

		if (dist > 0.001f)
		{
			//目標への単位ベクトルに変換
			float tx = vec.x / dist;
			float tz = vec.z / dist;

			//旋回率(小さいほど旋回が緩やかになる)
			float turnRate = 0.3f;

			float speed = 0.4;

			m_Velocity.x = (1 - turnRate) * m_Velocity.x + turnRate * tx * speed;

			m_Velocity.z = (1 - turnRate) * m_Velocity.z + turnRate * tz * speed;
			
			float vlen = sqrtf(m_Velocity.x * m_Velocity.x + m_Velocity.z * m_Velocity.z);

			if (vlen > 0.001)
			{
				m_Velocity.x /= vlen * speed;
				m_Velocity.z /= vlen * speed;
			}
		}
	}
}
