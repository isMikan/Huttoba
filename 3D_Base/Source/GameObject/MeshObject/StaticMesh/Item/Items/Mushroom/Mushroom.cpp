#include "Mushroom.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Mushroom>(ItemID::Mushroom); }

Mushroom::Mushroom()
	: m_UseDuration		( 20 )		//値を変えるとアイテムの使用可能時間が変化
	, m_IsPlaced		( false )

	, m_Velocity		()
	, m_MoveSpeed		( 6.0f )	//値を変えると移動速度が変化
	, m_UpSpeed			( 5.0f )	//値を変えると爆弾のy軸の上昇量が変化

	, m_IsThrow			( false )

	, m_MinSmashPower	( 6.0f )
	, m_MaxSmashPower	( 7.0f )

	, m_hEffect			()
	, m_ThrowHitPlayer	()

{
	Init();
}

Mushroom::~Mushroom()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());

	AssetManager::Effect()->Stop(m_hEffect);
}

void Mushroom::Init()
{
	m_UseTime = m_UseDuration;
	//ゲージのために追加.	制作者	[甲把]
	m_UsageLimit = { m_UseTime, m_UseDuration };

	AttachMesh(AssetManager::Mesh(StaticMeshList::Mushroom));

	SetPosition( 3.0f, 15.0f, 0.0f );

	m_State = IItemObserver::IItemObserver::State::Spawn;

	m_tGravity = 0.001f;

	//当たり判定
	std::shared_ptr<CStaticMesh> UseMesh = AssetManager::Mesh(StaticMeshList::Mushroom);
	std::shared_ptr<CStaticMesh> PickMesh = AssetManager::Mesh(StaticMeshList::PickUpCol);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Mushroom,
		PickMesh,
		this
	);

	m_pUseCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Mushroom,
		UseMesh,
		this
	);

	m_pUseCollider->SetActive(false);

}

void Mushroom::Update()
{
	ItemBase::Update();
}

void Mushroom::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Mushroom::Spawn()
{
	//落下処理
	if (m_vPosition.y > 0.5f)
	{
		m_vPosition.y -= m_Velocity.y;
		m_Velocity.y += m_tGravity;
	}
	else
	{
		//状態を地面についたときに変更
		m_State = IItemObserver::IItemObserver::State::OnGround;
	}
}

void Mushroom::OnGround()
{

}

void Mushroom::Have()
{
	HaveMove();
}

void Mushroom::Use()
{
	UseMove();
}

void Mushroom::Throw()
{
	ThrowMove();
}

void Mushroom::Destroy()
{
	m_IsPlaced = false;
	DestroyItem();
}

void Mushroom::ItemState(IItemObserver::State state)
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

void Mushroom::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_IsPlaced)
			{
				Smash(*player);
			}

			if (m_State==IItemObserver::State::Throw&& m_pPlayer != player)
			{
				//すでに当たっていないか？
				auto it = std::find(m_ThrowHitPlayer.begin(), m_ThrowHitPlayer.end(), player);
				if (it != m_ThrowHitPlayer.end()) return;

				ThrowSmash(*player);
				m_ThrowHitPlayer.push_back(player);
			}
		}
	}
}

void Mushroom::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Mushroom::UseMove()
{
	if (m_vPosition.y > 0.1f)
	{
		m_tGravity += 0.001f;
		m_vPosition.y -= m_tGravity;
	}
	else
	{
		if (m_IsOnGround)
		{
			m_vPosition.y = 0.1f;

			m_Velocity = D3DXVECTOR3(0, 0, 0);

			if (!m_IsPlaced)
			{
				//アイテムを設置中に
				m_IsPlaced = true;

				//エフェクトの再生
				m_hEffect = AssetManager::Effect()->Play("mushroomarea", m_vPosition);
				//エフェクトの拡縮を設定
				AssetManager::Effect()->SetScale(m_hEffect, D3DXVECTOR3(0.2f, 0.2f, 0.2f));
			}
		}
	}

	//移動速度を減算するための0.98f
	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	m_UseTime -= CTimeManager::GetDeltaTime();

	m_UsageLimit.remaining = m_UseTime;

	if (m_IsPlaced)
	{
		m_timer += CTimeManager::GetDeltaTime();
		
		float scale = 1.5f + fabsf(sinf(m_timer)) * 0.2f;

		SetScale(scale);

		if (!AssetManager::Effect()->IsPlaying(m_hEffect))
		{
			//エフェクト再生
			m_hEffect = AssetManager::Effect()->Play("mushroomarea", m_vPosition);
			//エフェクトの拡縮を設定
			AssetManager::Effect()->SetScale(m_hEffect, D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		}
	}

	if (m_UseTime < 0)
	{
		m_IsPlaced = false;
		DestroyItem();
	}
}

void Mushroom::ThrowMove()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.3f)
	{
		m_IsPlaced = false;
		DestroyItem();
	}

	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void Mushroom::OneEnterUse()
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

	m_Velocity.y = m_UpSpeed;

	//投げた瞬間に別のアイテムを持ったり使ったりできるように追加
	m_pPlayer->SetHoldingItem(nullptr);

	m_pPickUpCollider->SetActive(false);
	m_pUseCollider->SetActive(true);
}

void Mushroom::OneEnterThrow()
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

	m_IsThrow = true;
}

void Mushroom::Smash(CPlayerBase& playiers)
{	
	//キノコとプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	//プレイヤーの吹き飛ばしの計算
	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, CalculateForceScalar(len), 50.0f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockback);
}

float Mushroom::CalculateForceScalar(float distance) const
{
	//半径を仮設定
	//当たり判定用メッシュの大きさにしたい
	float maxDist = 2;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//現状当たり範囲を指定してるので0.0~1.0の間に収まるかわからないのでクランプする
	ratio = std::clamp(ratio, 0.0f, 1.0f);

	float power = m_MinSmashPower + (m_MaxSmashPower - m_MinSmashPower) * ratio;

	return power;
}

D3DXVECTOR3 Mushroom::CalculateReflectionDirection(const D3DXVECTOR3& vIncomingDir, const D3DXVECTOR3& vNormal)
{
	//反射の計算
	//公式の : r = v - 2 * dot(v,n) * n を使う

	float dot = D3DXVec3Dot(&vIncomingDir, &vNormal);

	D3DXVECTOR3 reflectDir = vIncomingDir - 2.0f * dot * vNormal;

	D3DXVec3Normalize(&reflectDir, &reflectDir);

	return reflectDir;
}