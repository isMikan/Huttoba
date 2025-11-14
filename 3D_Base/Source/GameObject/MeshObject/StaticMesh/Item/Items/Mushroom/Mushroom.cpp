#include "Mushroom.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Mushroom>("Mushroom"); }

Mushroom::Mushroom()
	: m_IsPlaced		( false )

	, m_Velocity		()
	, m_MoveSpeed		( 6.0f )	//値を変えると爆弾の移動相度が変化

	, m_IsThrow			( false )

	, m_IsHasThrow		( false )

	, m_MinSmashPower	( 6.0f )
	, m_MaxSmashPower	( 7.0f )
{
	Init();
}

Mushroom::~Mushroom()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Mushroom::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Mushroom));

	SetPosition( 3.0f, 15.0f, 0.0f );

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.001f;

	m_UseCount = 1;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Mushroom);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Mushroom,
		mesh,
		this
	);
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
		m_State = ItemBase::State::OnGround;
	}
}

void Mushroom::OnGround()
{

}

void Mushroom::Have()
{
	//デバック用で何度でも投げれるように
	if (m_IsHasThrow)
	{
		m_IsHasThrow = false;
	}

	HaveMove();
}

void Mushroom::Use()
{
	UseMove();

	m_pPlayer->SetItemBase(nullptr);
}

void Mushroom::Throw()
{
	ThrowMove();

	//m_pPlayer->SetItemBase(nullptr);
}

void Mushroom::Destroy()
{
	m_IsDestroy = true;

	m_pPlayer->SetItemBase(nullptr);
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

			if (m_IsThrow)
			{
				Smash(*player);
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
	if (!m_IsHasThrow)
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

		m_Velocity.y = 5.0f;

		m_IsHasThrow = true;
	}

	if (m_vPosition.y > 0.05f)
	{
		m_tGravity += 0.001f;
		m_vPosition.y -= m_tGravity;
	}
	else
	{
		m_vPosition.y = 0;

		m_Velocity = D3DXVECTOR3(0, 0, 0);
		
		if (!m_IsPlaced)
			m_IsPlaced = true;
		
	}

	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

}

void Mushroom::ThrowMove()
{
	if (!m_IsHasThrow)
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

		m_IsHasThrow = true;
		
		m_IsThrow = true;
	}


	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity)<=0.3)
	{
		static ::EsHandle hEffect = 1;

		//エフェクト追加
		hEffect = AssetManager::Effect()->Play("Break", m_vPosition);

		//エフェクトの拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.3f, 0.3f, 0.3f));

		m_IsDestroy = true;

		m_pPlayer->SetItemBase(nullptr);
	}

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
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

float Mushroom::CalculateForceScalar(float distance)
{
	//キノコの当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 2;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

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