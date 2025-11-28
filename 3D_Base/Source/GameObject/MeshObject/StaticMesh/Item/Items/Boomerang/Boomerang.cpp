#include "stdafx.h"
#include "Boomerang.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

static constexpr int SMASH_POWER = 6;
static constexpr float SMASH_ANGLE = 60.f;

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Boomerang>(ItemID::Boomerang); }

Boomerang::Boomerang()
	: m_Velocity		()
	, m_TotalVelocity	()
	, m_MoveSpeed		( 8.0f )	//値を変えると爆弾の移動速度が変化
	, m_UpSpeed			( 5.0f )	//値を変えると爆弾のy軸の上昇量が変化	
	, m_IsUseThrow	( false )
	, m_ComeBack	( false )
{
	Init();
}

Boomerang::~Boomerang()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Boomerang::Init()
{
	m_ComeBack = false;


	AttachMesh(AssetManager::Mesh(StaticMeshList::Boomerang));

	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_tGravity = 0.01f;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Bomb,
		mesh,
		this
	);
}

void Boomerang::Update()
{
	ItemBase::Update();
}

void Boomerang::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Boomerang::Spawn()
{
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

void Boomerang::OnGround()
{
}

void Boomerang::Have()
{
	m_ComeBack = false;

	HaveMove();
}

void Boomerang::Use()
{
	UseMove();
}

void Boomerang::Throw()
{
	ThrowMove();
}

void Boomerang::Destroy()
{
	DestroyItem();
}

void Boomerang::ItemState(IItemObserver::State state)
{
	switch (state)
	{
	case IItemObserver::IItemObserver::State::Spawn:
		break;
	case IItemObserver::IItemObserver::State::OnGround:
		break;
	case IItemObserver::IItemObserver::State::Have:
		break;
	case IItemObserver::IItemObserver::State::Use:
		OneEnterUse();
		break;
	case IItemObserver::IItemObserver::State::Throw:
		OneEnterThrow();
		break;
	case IItemObserver::IItemObserver::State::Destroy:
		break;
	default:
		break;
	}
}

void Boomerang::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if ((m_State == State::Use || m_State == State::Throw )
				&& player != m_pPlayer)
			{
				Smash(*player);
			}
		}
	}
}

void Boomerang::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
}

void Boomerang::UseMove()
{
	if (!m_ComeBack)
	{
		//だんだん減速
		m_Velocity.x -= m_Velocity.x * 0.01f;
		m_Velocity.z -= m_Velocity.z * 0.01f;
	}
	else
	{
		D3DXVECTOR3 vector = m_pPlayer->GetPosition() - m_vPosition;
		D3DXVECTOR3 initVector;
		D3DXVec3Normalize(&initVector, &vector);

		//だんだん加速
		m_Velocity = initVector * m_MoveSpeed;

		m_Velocity.x += m_Velocity.x * 0.25f;
		m_Velocity.z += m_Velocity.z * 0.25f;
	}

	//推進力が一定まで下がるとPlayerに戻る
	if (std::fabs(m_Velocity.x) < 2.f && std::fabs(m_Velocity.z) < 2.f)
	{
		m_ComeBack = true;
	}

	//位置を移動速度*デルタタイムで計算
	m_vPosition += m_Velocity * CTimeManager::GetDeltaTime();
	m_TotalVelocity += m_Velocity * CTimeManager::GetDeltaTime();;

	//回転
	m_vRotation.x = m_vRotation.x + (D3DXToRadian(10.f));

	//使用フラグをオンに
	m_IsUseThrow = true;

}

void Boomerang::ThrowMove()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.3f)
	{
		DestroyItem();
	}

	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void Boomerang::OneEnterUse()
{
	//プレイヤーのクォータニオン(向いている方向)記録
	m_vQuaternion = m_pPlayer->GetQuaternion();

	D3DXMATRIX matRot;

	//クォータニオンをマトリックス(行列)に変換
	D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);

	//行列の中にあるZ軸成分を取り出す
	//D3DXVECTOR3 forward = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//取り出したZ軸成分をノーマライズ
	//D3DXVec3Normalize(&forward, &forward);

	D3DXVECTOR3 forward = m_pPlayer->GetLocalAxes().forward;

	//移動
	m_Velocity = forward * m_MoveSpeed;

	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::BoomerangCol);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Boomerang,
		mesh,
		this
	);

	m_IsOkFall = false;
}

void Boomerang::OneEnterThrow()
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
}


void Boomerang::Smash(CPlayerBase& playiers)
{
	//爆弾とプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	//プレイヤーの吹き飛ばしの計算
	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, SMASH_POWER, SMASH_ANGLE);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);

	static ::EsHandle hEffect = 1;

	//エフェクト追加
	hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

}