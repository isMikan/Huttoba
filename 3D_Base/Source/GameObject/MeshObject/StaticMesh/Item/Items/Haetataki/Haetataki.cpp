#include "stdafx.h"
#include "Haetataki.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

//--------------------------------------------------------------------------------------------------------------

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Haetataki>(ItemID::Haetataki); }

//--------------------------------------------------------------------------------------------------------------

//汎用定数宣言

//プレイヤーとの位置を合わせるためのオフセット
constexpr float OFFSET_X = 0.5f;	
constexpr float OFFSET_Y = 1.f;		

//モーションの移動加速度
constexpr float ADD_POS_X = 0.02f;

//モーションの回転加速度
constexpr float ADD_ROT_X = 0.2f;		
constexpr float ADD_ROT_Y = 0.2f;
constexpr float ADD_ROT_Z = 0.2f;

constexpr float THROW_SPEED = 6.0f;

//初期位置
constexpr float INITAL_POS_X = 0.f;		
constexpr float INITAL_POS_Y = 5.f;
constexpr float INITAL_POS_Z = 5.f;

//初期角度
constexpr float INITAL_ROT_X = 0.f;
constexpr float INITAL_ROT_Y = 0.f;
constexpr float INITAL_ROT_Z = 90.f;

//重力関連
constexpr float INITAL_GRAVITY = 0.01f;
constexpr float ADD_GRAVITY = 0.001f;

//ステージの高さ(当たり判定ができたら消す)突貫
constexpr float STAGE_HEIGHT = 0.5f;

constexpr float OFFSET_USE_COLLISION_X = 0.0f;
constexpr float OFFSET_USE_COLLISION_Y = 0.0f;
constexpr float OFFSET_USE_COLLISION_Z = 0.0f;

//使用上限
constexpr float USE_COUNT = 7;	

// 回転にかける総時間
constexpr float SLERP_DURATION = 1.0f;

//回数制限
constexpr float USE_LIMIT = 7;		
//--------------------------------------------------------------------------------------------------------------

Haetataki::Haetataki()
	: m_SwitchDir		( false )
	, m_IsFlyAway		( false )
	, m_IsFlyAwayPower	( 3.f )
	, m_IsMissAttack	( false )
	, m_Velocity		()
	, m_slerpTime		()
	, m_InitalPlayerQ	()
	, m_IsFirst			()
	, m_Startfix		()
	, m_Endfix			()
	, m_HitPlayer		()
	, m_ThrowHitPlayer	()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------

Haetataki::~Haetataki()
{
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Init()
{
	m_UseCount = USE_LIMIT;
	m_UsageLimit = { m_UseCount, USE_LIMIT };

	AttachMesh(AssetManager::Mesh(StaticMeshList::Haetataki));

	SetPosition(INITAL_POS_X, INITAL_POS_Y, INITAL_POS_Z);

	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_tGravity = INITAL_GRAVITY;


	//当たり判定
	std::shared_ptr<CStaticMesh> UseMesh = AssetManager::Mesh(StaticMeshList::HaetatakiCol);
	std::shared_ptr<CStaticMesh> PickMesh = AssetManager::Mesh(StaticMeshList::PickUpCol);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Haetataki,
		PickMesh,
		this
	);

	m_pUseCollider = CollisionDataFactory::CreateCapsuleForMesh(
		CollisionBase::ColliderTag::Haetataki,
		UseMesh,
		this
	);

	//攻撃用判定はいったん使用しない
	m_pUseCollider->SetActive(false);

	D3DXVECTOR3 UseOffset = { OFFSET_USE_COLLISION_X,OFFSET_USE_COLLISION_Y,OFFSET_USE_COLLISION_Z };

	m_pUseCollider->SetLocalOffSetToCapsule(UseOffset, UseOffset);

	m_ObjectColor.resize(2);

	//赤色のカラーコード
	D3DXVECTOR4 red = D3DXVECTOR4(1.0f, 0.2f, 0.2f, 1.0f);
	m_ObjectColor[0].diffuse = red;
	m_ObjectColor[1].diffuse = red;

	D3DXQuaternionRotationYawPitchRoll(&m_vQuaternion, D3DXToRadian(90.0f), D3DXToRadian(90.0f), 0.0f);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Update()
{
	//アイテム共通のUpdate
	ItemBase::Update();
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//アイテム共通のDraw
	ItemBase::Draw(View, Proj, Light, Camera);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Spawn()
{
	//落下処理
	if(m_vPosition.y > STAGE_HEIGHT)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += ADD_GRAVITY;
	}
	else
	{
		m_State = IItemObserver::IItemObserver::State::OnGround;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::OnGround()
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Have()
{
	//アイテムを拾うモーション
	TakeMostion();

	//位置を合わせる
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	// プレイヤーの回転
	D3DXQUATERNION playerQ = m_pPlayer->GetQuaternion();


	static float a = 0.f, b = 0.f, c = 0.f;

	// ハエたたきの補正角
	D3DXQUATERNION fix;
	//D3DXQuaternionRotationYawPitchRoll(&fix, D3DXToRadian(a), D3DXToRadian(b), D3DXToRadian(c));
	D3DXQuaternionRotationYawPitchRoll(&fix, D3DXToRadian(90.f), 0, D3DXToRadian(45.f));

	D3DXQUATERNION finalQ;
	D3DXQuaternionMultiply(&finalQ, &fix, &playerQ);
	m_vQuaternion = finalQ;

	if (!m_IsFirst) { m_IsFirst = true; }

	m_SwitchDir = false;
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Use()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_pPickUpCollider->SetActive(false);
	m_pUseCollider->SetActive(true);


	//モーション終了で所持状態へ戻る
	if (!AttackMostion())
	{
		m_State = IItemObserver::IItemObserver::State::Have;
		m_IsMissAttack = false; //初期化

		m_pPickUpCollider->SetActive(true);
		m_pUseCollider->SetActive(false);
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Throw()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.6f)
	{
		DestroyItem();
	}

	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Destroy()
{
	DestroyItem();
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::ItemState(IItemObserver::State state)
{
	switch (state)
	{
	case IItemObserver::State::Have:
		m_HitPlayer.clear();
		if (m_UseCount <= 0) { Destroy(); }

		break;
	case IItemObserver::State::Use:
		m_UsageLimit.remaining = --m_UseCount;
		AssetManager::Sound()->PlaySE(enSoundList::SE_MissHaetataki);

		break;
	case IItemObserver::State::Throw:
		OneEnterThrow();
		break;
	case IItemObserver::State::Destroy:
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::TakeMostion()
{
}

//--------------------------------------------------------------------------------------------------------------

bool Haetataki::AttackMostion()
{
	// プレイヤーの回転
	D3DXQUATERNION playerQ = m_pPlayer->GetQuaternion();

	// モーション開始時の初期化
	if (m_IsFirst)
	{
		m_InitalPlayerQ = playerQ; // プレイヤーの開始時の向きを固定
		m_slerpTime = 0.0f;
		m_IsFirst = false;
	}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	D3DXQUATERNION startQ = { D3DXToRadian(40.f), D3DXToRadian(45.f), D3DXToRadian(-51.5f), 1.0f };
	D3DXQUATERNION endQ = { D3DXToRadian(40.f), D3DXToRadian(8.f), D3DXToRadian(61.5f), 1.0f };

	startQ *= m_InitalPlayerQ;
	endQ  *= m_InitalPlayerQ;


	m_slerpTime += (CTimeManager::GetDeltaTime()) * 5; // 時間経過を早める（*5）

	float t = m_slerpTime / SLERP_DURATION;
	if (t > 1.0f)
	{
		t = 1.0;
	}

	// 球面線形補間でアイテムの現在の回転を更新
	D3DXQuaternionSlerp(
		&m_vQuaternion,
		&startQ,
		&endQ,
		t
	);

	// 回転が完了した場合
	if (t >= 1.0f)
	{
		m_slerpTime = 0.0f;
		m_IsFirst = true; // 次の攻撃のためにリセット
		std::cout << "モーション終了" << std::endl;
		return false; // モーション終了
	}

	// モーション中
	return true;
}
//--------------------------------------------------------------------------------------------------------------

void Haetataki::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_pPlayer != player && m_State == IItemObserver::State::Use)
			{
				//すでに当たっていないか？
				auto it = std::find(m_HitPlayer.begin(), m_HitPlayer.end(), player);
				if (it != m_HitPlayer.end()) return;

				Smash(*player);
				m_HitPlayer.push_back(player);
				AssetManager::Sound()->PlaySE(enSoundList::SE_HitHaetataki);

				//エフェクト設定
				D3DXVECTOR3 EffectPos = player->GetPosition();
				EffectPos.y += 1.5f;
				static ::EsHandle hEffect = 1;
				hEffect = AssetManager::Effect()->Play("BoomerangHit", EffectPos);
				AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(1.f, 1.f, 1.f));

			}

			if (m_State == IItemObserver::State::Throw && m_pPlayer != player)
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

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Smash(CPlayerBase& playiers)
{
	//爆弾とプレイヤーの位置でベクトルをとる
	D3DXVECTOR3 vecLen = m_vPosition - playiers.GetPosition();

	//ベクトルを長さに変換
	float len = D3DXVec3Length(&vecLen);

	//プレイヤーの吹き飛ばしの計算
	D3DXVECTOR3 SmashVel = playiers.GetKnockbackVelocity(m_vPosition, 8, 60.0f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Knockdown);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::OneEnterThrow()
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

	//m_Velocity = forward * THROW_SPEED;

	m_Velocity = forward * m_ThrowSpeed;

	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}