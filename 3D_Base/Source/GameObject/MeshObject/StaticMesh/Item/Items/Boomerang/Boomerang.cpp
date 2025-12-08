#include "stdafx.h"
#include "Boomerang.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

constexpr float USE_LIMIT = 5;		//回数制限
constexpr float MOVE_SPEED = 5.f;	//最低限の飛ぶ速さ

constexpr float COMEBACK_SPEED = 2.f;	//この速度以下で切り替えす

constexpr float TURN_ANGLE_RAD = 10.f;	//この速度以下で切り替えす

constexpr int SMASH_POWER = 6;		//吹っ飛び力
constexpr float SMASH_ANGLE = 60.f; //吹っ飛び角度

constexpr float MAX_CHARGE = 5.f;		  //チャージ上限
constexpr float ADD_CHARGE_RANGE = 0.05f; //チャージしてる間の1f間の上昇量

constexpr float USE_MIN_VELOCITY_RANGE = 0.01f;				//飛ばしている間の1f間の減衰
constexpr float COMEBACK_ADD_VELOCITY_RANGE = 0.25f;	//戻ってくるときの1f間の速度上昇量

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Boomerang>(ItemID::Boomerang); }

Boomerang::Boomerang()
	: m_Velocity		()
	, m_AddVelocity		()
	, m_TotalVelocity	()
	, m_MoveSpeed		( MOVE_SPEED )	//値を変えると爆弾の移動速度が変化
	, m_IsUseThrow	( false )
	, m_ComeBack	( false )
	, m_IsCharge	( false )
	, m_IsMaxCharge	( false )
	, m_hEffect()
{
	Init();
}

Boomerang::~Boomerang()
{
	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
}

void Boomerang::Init()
{
	//ゲージ
	m_UseCount = MOVE_SPEED;
	m_UsageLimit = { m_UseCount, USE_LIMIT };

	//各種フラグ
	m_ComeBack = false;
	m_IsCharge = false;
	m_IsMaxCharge = false;

	//メッシュをアタッチ
	AttachMesh(AssetManager::Mesh(StaticMeshList::Boomerang));

	//判定作成
	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Bomb,
		mesh,
		this
	);

	mesh = AssetManager::Mesh(StaticMeshList::BoomerangCol);

	m_pUseCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Boomerang,
		mesh,
		this
	);

	m_pUseCollider->SetActive(false);

	//ステート設定
	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_tGravity = 0.01f;


	m_AddVelocity = { 0.f,0.f,0.f };
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
	//帰ってきたときに取得音を鳴らす
	if(m_IsUseThrow == true){ AssetManager::Sound()->PlaySE(enSoundList::SE_Pickup);}

	//帰ってきたときの初期化
	if ( m_ComeBack	  )	{ m_ComeBack	= false;}
	if ( m_IsUseThrow )	{ m_IsUseThrow	= false;}
	if ( m_IsCharge	  )	{ m_IsCharge	= false;}
	if ( m_IsMaxCharge)	{ m_IsMaxCharge = false;}

	m_AddVelocity = { 0.f,0.f,0.f };
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
		if (m_UseCount <= 0){ Destroy(); }

		m_pUseCollider->SetActive(false);
		m_pPickUpCollider->SetActive(true);

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
	if (!m_pPlayer)return;

	//押されている間は発射せずにチャージ
	if (CInputManager::IsRepeat(Action::Attack,m_pPlayer->GetPlayerID()) && !m_IsUseThrow)
	{
		//チャージできるか
		if (m_AddVelocity.x < MAX_CHARGE && m_AddVelocity.z < MAX_CHARGE)
		{
			m_AddVelocity.x += ADD_CHARGE_RANGE;
			m_AddVelocity.z += ADD_CHARGE_RANGE;

			//チャージのサウンド
			if (!m_IsCharge)	{AssetManager::Sound()->PlaySE(enSoundList::SE_BoomerangCharge);}

			m_IsCharge = true;
		}
		//最大チャージ
		else
		{
			//エフェクト追加
			if (!AssetManager::Effect()->IsPlaying(m_hEffect[Efect::ChargeMax]))
			{
				m_hEffect[Efect::ChargeMax] = AssetManager::Effect()->Play("BoomerangMaxCharge", m_vPosition);
			}

			//エフェクトの拡縮設定
			AssetManager::Effect()->SetScale(m_hEffect[Efect::ChargeMax], D3DXVECTOR3(0.5f, 0.5f, 0.5f));
			AssetManager::Effect()->SetLocation(m_hEffect[Efect::ChargeMax],m_pPlayer->GetPosition());
			AssetManager::Sound()->Stop(enSoundList::SE_BoomerangCharge);


			if (!m_IsMaxCharge){AssetManager::Sound()->PlaySE(enSoundList::SE_BoomerangMaxCharge);}

			m_IsMaxCharge = true;
			m_IsCharge = false;

		}
		m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

		//プレイヤーのクォータニオン(向いている方向)記録
		m_vQuaternion = m_pPlayer->GetQuaternion();

		D3DXMATRIX matRot;

		//クォータニオンをマトリックス(行列)に変換
		D3DXMatrixRotationQuaternion(&matRot, &m_vQuaternion);

		D3DXVECTOR3 forward = m_pPlayer->GetLocalAxes().forward;

		//移動
		m_Velocity.x = forward.x * (m_MoveSpeed + m_AddVelocity.x);
		m_Velocity.z = forward.z * (m_MoveSpeed + m_AddVelocity.z);

	}
	else
	{
		//戻ってくるフラグによって動作変更
		if (!m_ComeBack)
		{
			//だんだん減速
			m_Velocity.x -= m_Velocity.x * USE_MIN_VELOCITY_RANGE;
			m_Velocity.z -= m_Velocity.z * USE_MIN_VELOCITY_RANGE;
		}
		else
		{
			//プレイヤーに戻ってくる用にベクトル取得
			D3DXVECTOR3 vector = m_pPlayer->GetPosition() - m_vPosition;
			D3DXVECTOR3 initVector;
			D3DXVec3Normalize(&initVector, &vector);

			//戻ってくる初速度
			m_Velocity = initVector * (m_MoveSpeed * 2);

			//だんだん戻ってくる速さ増加
			m_Velocity.x += m_Velocity.x * COMEBACK_ADD_VELOCITY_RANGE;
			m_Velocity.z += m_Velocity.z * COMEBACK_ADD_VELOCITY_RANGE;
		}

		//推進力が一定まで下がるとPlayerに戻る
		if (std::fabs(m_Velocity.x) < COMEBACK_SPEED && std::fabs(m_Velocity.z) < COMEBACK_SPEED)
		{
			m_ComeBack = true;
		}

		//位置を移動速度*デルタタイムで計算
		m_vPosition += m_Velocity * CTimeManager::GetDeltaTime();
		m_TotalVelocity += m_Velocity * CTimeManager::GetDeltaTime();;

		//回転
		m_vRotation.x = m_vRotation.x + (D3DXToRadian(TURN_ANGLE_RAD));

		//投げる瞬間に一回だけ通す処理
		if (!m_IsUseThrow)
		{ 
			m_UsageLimit.remaining = --m_UseCount; 
			m_pUseCollider->SetActive(true);
			m_pPickUpCollider->SetActive(false);
		}
		
		//使用フラグをオンに
		if(!m_IsUseThrow){ AssetManager::Sound()->PlaySE(enSoundList::SE_BoomerangThrow); }
		m_IsUseThrow = true;

		//チャージのSE消す
		AssetManager::Sound()->Stop(enSoundList::SE_BoomerangCharge);
	}
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

	D3DXVECTOR3 forward = m_pPlayer->GetLocalAxes().forward;

	//移動
	m_Velocity = forward * m_MoveSpeed;

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

	//エフェクト追加
	m_hEffect[Efect::HitPlayer] = AssetManager::Effect()->Play("Explosion", m_vPosition);

	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(m_hEffect[Efect::HitPlayer], D3DXVECTOR3(0.6f, 0.6f, 0.6f));

}