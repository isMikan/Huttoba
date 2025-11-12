#include "Mushroom.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Mushroom>("Mushroom"); }

Mushroom::Mushroom()
	: m_IsPlaced		(false)

	, m_HaveOffset		()

	, m_Velocity		()
	, m_MoveSpeed		( 6.0f )	//値を変えると爆弾の移動相度が変化

	, m_IsThrow			( true )

	, m_MinSmashPower	( 6.0f )
	, m_MaxSmashPower	( 15.0f )
{
	Init();
}

Mushroom::~Mushroom()
{
}

void Mushroom::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Mushroom));

	SetPosition( 3.0f, 15.0f, 0.0f );

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;

	m_HaveOffset = D3DXVECTOR3(0.0f, 0.2f, 0.0f);

	m_UseCount = 1;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

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
	if (m_vPosition.y > 1.2f)
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

void Mushroom::OnGround()
{

}

void Mushroom::Have()
{
	if (!m_IsThrow)
	{
		m_IsThrow = true;
		m_tGravity = 9.8f;
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

		m_IsThrow = false;
	}


	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	if (m_vPosition.y > 0.5f)
	{
		m_tGravity += 0.001f;
		m_vPosition.y -= m_tGravity;
	}
	else
	{
		m_vPosition.y = 0;
	}

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void Mushroom::Destroy()
{
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
		}
	}
}

void Mushroom::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition() + m_HaveOffset;
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Mushroom::UseMove()
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

		m_Velocity.y = 5.0f;

		m_tGravity=

		m_IsThrow = false;
	}

	if (m_vPosition.y > 0.5f)
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
	UseMove();
}

void Mushroom::Hit()
{
	////プレイヤーとキノコのぶつかった方向のベクトルを取得
	//D3DXVECTOR3 normal = m_pPlayer->GetPosition() - m_vPosition;

	////ノーマライズして法線ベクトルを取得
	//D3DXVec3Normalize(&normal, &normal);

	////プレイヤーの移動方向を取得
	//D3DXVECTOR3 velPlayer = m_pPlayer->GetKnockbackVelocity();

	//D3DXVec3Normalize(&velPlayer, &velPlayer);

	////反射方向を記録
	//D3DXVECTOR3 reflectDir = CalculateReflectionDirection(m_pPlayer->GetKnockbackVelocity(), normal);


	//D3DXVECTOR3 a = m_pPlayer->GetPosition() - m_vPosition;
	////ノックバックの強さを計算
	//float len = D3DXVec3Length(&a);
	////距離に応じてパワー計算
	//float knockbackPower = CalculateForceScalar(len);

}

void Mushroom::Smash(CPlayerBase& playiers)
{	
	//爆弾とプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	//プレイヤーの吹き飛ばしの計算
	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, CalculateForceScalar(len), 60.0f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);
}

float Mushroom::CalculateForceScalar(float distance)
{
	//線形補間で計算

	//キノコの当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 2;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	ratio = std::clamp(ratio, 0.0f, 1.0f);

	//最小吹き飛ばし力
	float minPower = 6.0f;

	//最大吹き飛ばし力
	float maxPower = 15.0f;

	float power = minPower + (maxPower - minPower) * ratio;

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