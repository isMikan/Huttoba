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

	, m_ColorTimer		( 0.0 )
	
	, m_IsExploded		( false )

	, m_MinSmashPower	( 5.0f )	//値を変えるとプレイヤーの最小吹き飛ばし力が変化

	, m_MaxSmashPower	( 10.0f )	//値を変えるとプレイヤーの最大吹き飛ばし力が変化
{
	Init();
	m_ObjectColor.resize(2);

	//爆弾の爆弾部分の灰色の値
	m_ObjectColor[0].diffuse = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	//拡散反射だけではいい感じにならなかったので環境光も変化
	m_ObjectColor[0].ambient = D3DXVECTOR4(0.15f, 0.15f, .15f, 1.f);

	//爆弾の紐の部分の白色の値
	m_ObjectColor[1].diffuse = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	//m_ObjectColor[1].ambient = D3DXVECTOR4(.3f, .3f, .3f, .5f);

}

Bomb::~Bomb()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Bomb::Init()
{
	static const int USE_COUNT = 1;

	AttachMesh(AssetManager::Mesh(StaticMeshList::Bomb));

	m_State = ItemBase::State::Spawn;
	m_tGravity = 0.01f;
	m_UseCount = USE_COUNT;

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		this,
		mesh,
		CollisionBase::ColliderTag::Bomb
	);
}

void Bomb::Update()
{
	ItemBase::Update();
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
	//if (GetAsyncKeyState('M') & 0x0001)
	////if(CInputManager::IsDown(Action::Have,0))
	//{
	//	//状態を取得中に変化
	//	m_State = ItemBase::State::Have;

	//	//プレイヤー側にあるモーションと同期できるように
	//	//m_IsTake = true;
	//}
}

void Bomb::Have()
{
	//if (m_IsTake)
		//TakeMotion();
	//else
	HaveMove();
}

void Bomb::Use()
{
	UseMove();

	m_pPlayer->SetItemBase(nullptr);
}

void Bomb::Throw()
{
	UseMove();
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
			if (m_IsExploded)
			{
				Smash(*player);
			}
		}
	}
}

void Bomb::HaveMove()
{
<<<<<<< HEAD
	m_PickUpCnt += CTimeManager::GetDeltaTime();

	if (m_PickUpCnt >= m_PickUpTime)
	{
		m_IsTake = false;
	}
}

void Bomb::PossessionMotion()
{
=======
>>>>>>> bomb
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
}

void Bomb::UseMove()
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

		//当たり判定削除
		CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

		std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::ExplosionCol);

		m_pCollision = CollisionDataFactory::CreateSphereForMesh(
			this,
			mesh,
			CollisionBase::ColliderTag::Bomb
		);
	}

	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	if (m_vPosition.y > 0.5f)
	{
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
}

void Bomb::ThrowMove()
{
	//投げる動作が使う動作と同じなのでこの処理
	UseMove();
}

void Bomb::Explosion()
{
	//爆発時に一度だけ処理する
	if (!m_IsExploded)
	{
		//爆発フラグをオンに
		m_IsExploded = true;

		static ::EsHandle hEffect = 1;

		//エフェクト追加
		hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

		//エフェクトの拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

		//アイテムの状態を破棄にする
		m_State = ItemBase::State::Destroy;
	}
}

void Bomb::Smash(CPlayer& playiers)
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

void Bomb::ChangeColor()
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

	//D3DXのVec4の線形補間の計算
	D3DXVec4Lerp(&color, &gray, &red, blinkRate);

	m_ObjectColor[0].diffuse = color;
}

float Bomb::CalculateForceScalar(float distance)
{
	//爆発の当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 2;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//線形補間の計算
	float power = m_MinSmashPower + (m_MaxSmashPower - m_MinSmashPower) * ratio;

	return power;
}