#include "stdafx.h"
#include "Boomerang.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Boomerang>(ItemID::Boomerang); }

Boomerang::Boomerang()
	: m_Velocity		()
	, m_TotalVelocity	()
	, m_MoveSpeed		( 8.0f )	//値を変えると爆弾の移動速度が変化
	, m_UpSpeed			( 5.0f )	//値を変えると爆弾のy軸の上昇量が変化

	, m_ExplosionTime	( 5.0f )	//値を変えると爆発するまでの時間が変化
	, m_ExplosionCnt	( 0.0f )

	, m_ColorTimer		( 0.0 )
	
	, m_IsUseThrow	( false )
	, m_ComeBack	( false )

	, m_MinSmashPower	( 6.0f )	//値を変えるとプレイヤーの最小吹き飛ばし力が変化

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

Boomerang::~Boomerang()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Boomerang::Init()
{
	static const int USE_COUNT = 1;

	AttachMesh(AssetManager::Mesh(StaticMeshList::Boomerang));

	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_tGravity = 0.01f;
	m_UseCount = USE_COUNT;
	//ゲージのために追加.	制作者	[甲把]
	m_UsageLimit = { USE_COUNT, USE_COUNT };

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
	m_IsDestroy = true;
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
			if (m_IsUseThrow && player != m_pPlayer)
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
	//位置を移動速度*デルタタイムで計算
	m_vPosition += m_Velocity * CTimeManager::GetDeltaTime();
	m_TotalVelocity += m_Velocity * CTimeManager::GetDeltaTime();;

	m_Velocity.x -= m_Velocity.x * 0.01;
	m_Velocity.z -= m_Velocity.z * 0.01;

	if (m_Velocity.x < 2.f && m_Velocity.z < 2.f)
	{
		m_vPosition = m_pPlayer->GetPosition();
	}

	UseThrow();
}

void Boomerang::ThrowMove()
{
	//投げる動作が使う動作と同じなのでこの処理
	UseMove();
}

void Boomerang::OneEnterUse()
{
	EnterUseThrowCommon();

	//投げた瞬間に別のアイテムを持ったり使ったりできるように追加
	m_pPlayer->SetItemBase(nullptr);
}

void Boomerang::OneEnterThrow()
{
	EnterUseThrowCommon();
}

void Boomerang::EnterUseThrowCommon()
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

void Boomerang::UseThrow()
{
	//使用フラグをオンに
	m_IsUseThrow = true;
}

void Boomerang::Smash(CPlayerBase& playiers)
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

	static ::EsHandle hEffect = 1;

	//エフェクト追加
	hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

}

void Boomerang::ChangeColor()
{
	m_ColorTimer += CTimeManager::GetDeltaTime();

	//点滅のスピードを経過時間/爆発するまでの時間をして割合で出す
	float speed = 10.0f * (static_cast<float>((m_ColorTimer) / m_ExplosionTime));

	//+1.0fをすることで、sinの値が0~2の間の値になり、*0.5することで0~1の間の値が取れる
	float blinkRate = (sinf(static_cast<float>(m_ColorTimer) * speed) + 1.0f) * 0.5f;

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

float Boomerang::CalculateForceScalar(float distance)
{
	//爆発の当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 1.8f;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//線形補間の計算
	float power = m_MinSmashPower + (m_MaxSmashPower - m_MinSmashPower) * ratio;

	return power;
}