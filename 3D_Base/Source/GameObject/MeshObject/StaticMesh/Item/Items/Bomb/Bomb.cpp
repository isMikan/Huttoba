#include "stdafx.h"
#include "Bomb.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

#include <PlayerBase/Player/CPlayer.h>

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Bomb>("Bomb"); }

Bomb::Bomb()
	: m_IsTake			( false )
	, m_PickUpTime		( 0.3f )	//値を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt		( 0.0f )

	, m_Velocity		()
	, m_MoveSpeed		( 3.0f )	//値を変えると爆弾の移動相度が変化

	, m_IsThrow			( true )

	, m_ExplosionTime	( 5.0f )	//値を変えると爆発するまでの時間が変化
	, m_ExplosionCnt	( 0.0f )

	, m_KnockBackPower	( 10.0f )	//値を変えるとプレイヤーの吹き飛ばし力が変化

	, m_ColorTimer		( 0.0 )
	
	, m_OneExplosion	( false )

	, m_tamesi			( false )
{
	Init();
}

Bomb::~Bomb()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Bomb::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Bomb));
	//AttachMesh(AssetManager::Mesh(StaticMeshList::ExplosionCol));

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::ExplosionCol);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
			this,
			mesh,
			CollisionBase::ColliderTag::Bomb
		);
}

void Bomb::Update()
{
	ItemBase::Update();

	std::cout << (m_IsOnGround ? "true" : "false") << std::endl;

}

void Bomb::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Bomb::Spawn()
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
		m_State = ItemBase::State::OnGround;
	}
}

void Bomb::OnGround()
{
	if (GetAsyncKeyState('M') & 0x0001)
	//if(CInputManager::IsDown(Action::Have,0))
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		//プレイヤー側にあるモーションと同期できるように
		//m_IsTake = true;
	}
}

void Bomb::Have()
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion();
}

void Bomb::Use()
{
	UseAndThrow();
}

void Bomb::Throw()
{
	UseAndThrow();
}

void Bomb::Destroy()
{
	m_IsDestroy = true;
}

void Bomb::OnCollision(CollisionBase* other)
{
	
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayer* player = dynamic_cast<CPlayer*>(other->GetListener()))
		{
			if (m_tamesi)
			{
				Blow_Away(*player);
			}
		}
	}
}

void Bomb::TakeMotion()
{
	m_PickUpCnt += CTimeManager::GetDeltaTime();

	if (m_PickUpCnt >= m_PickUpTime)
	{
		m_IsTake = false;
	}
}

void Bomb::PossessionMotion()
{

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

}

void Bomb::UseMotion()
{
}

void Bomb::ThrowMotion()
{
}

void Bomb::UseAndThrow()
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

		m_Velocity.y = 15.0f;

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
		Explosion();
	}

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	ChangeColor();

	//m_ExplosionCnt += CTimeManager::GetDeltaTime();

	//if (m_ExplosionCnt >= m_ExplosionTime)
	//{
	//	Explosion(playiers);
	//}
}

void Bomb::Explosion()
{
	if (!m_OneExplosion)
	{
		//Blow_Away(playiers);
		m_tamesi = true;
		m_OneExplosion = true;

		static ::EsHandle hEffect = -1;

		hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

		//拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

		m_State = ItemBase::State::Destroy;
	}
}

void Bomb::Blow_Away(CPlayer& playiers)
{
	D3DXVECTOR3 vecLen = m_vPosition - GetPosition();

	float len = D3DXVec3Length(&vecLen);

	//playiers.SetHitInfo(
	//	m_vPosition, GetPosition(),
	//	CalculateForceScalar(len),
	//	true, CPlayerBase::HitEvent::Knockdown);
}

void Bomb::ChangeColor()
{
	//赤色
	//m_pMesh->SetMaterialColor(0, D3DXVECTOR4(1, 0, 0, 1));
	//黒色(全て0.5が元の色)
	//m_pMesh->SetMaterialColor(0, D3DXVECTOR4(.5f, .5f, .5f, .5f));

	m_ColorTimer += CTimeManager::GetDeltaTime();

	//点滅のスピードを経過時間/爆発するまでの時間をして割合で出す
	double speed = 10.0f * (m_ColorTimer / m_ExplosionTime);

	//+1.0fをすることで、sinの値が0~2の間の値になり、*0.25で0~0.5の値がtに入る
	double blinkRate = (sin(m_ColorTimer * speed) + 1.0) * 0.25;

	//カラー増加変数
	float up = std::clamp(0.5f + static_cast<float>(blinkRate), .5f, 1.0f);

	//カラー減少変数
	float down = std::clamp(0.5f - static_cast<float>(blinkRate), 0.0f, 0.5f);

	//値が増加と減少がそれぞれあるので使いわけていく
	D3DXVECTOR4 color = D3DXVECTOR4(up, down, down, up);

	m_pMesh->SetMaterialColor(0, color);
}

float Bomb::CalculateForceScalar(float distance)
{
	//線形補間で計算

	//爆発の当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 6;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//爆発の最小吹き飛ばし力
	float minPower = 6.0f;

	//爆発の最大吹き飛ばし力
	float maxPower = m_KnockBackPower;

	float power = minPower + (maxPower - minPower) * ratio;

	return power;
}