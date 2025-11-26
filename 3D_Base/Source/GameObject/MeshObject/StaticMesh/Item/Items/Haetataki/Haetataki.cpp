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
constexpr float STAGE_HEIGHT = 1.2f;

//--------------------------------------------------------------------------------------------------------------

Haetataki::Haetataki()
	: m_Offset			( OFFSET_X, OFFSET_Y, 0.f )
	, m_AddPos			( 0.f, 0.f, 0.f )
	, m_AddRot			( ADD_ROT_X, ADD_ROT_Y, 0.f )
	, m_SwitchDir		( false )
	, m_IsFlyAway		( false )
	, m_IsFlyAwayPower	( 3.f )
	, m_IsMissAttack	( false )
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------

Haetataki::~Haetataki()
{
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Init()
{
	static const int USE_COUNT = 7;	//使用カウント

	AttachMesh(AssetManager::Mesh(StaticMeshList::Haetataki));

	SetPosition(INITAL_POS_X, INITAL_POS_Y, INITAL_POS_Z);
	SetRotation(INITAL_ROT_X, INITAL_ROT_Y, INITAL_ROT_Z);

	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_UseCount = USE_COUNT;
	m_tGravity = INITAL_GRAVITY;


	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Haetataki,
		mesh,
		this,
		false
	);

	mesh = AssetManager::Mesh(StaticMeshList::BCapsule);

	m_pUseCollider = CollisionDataFactory::CreateCapsuleForMesh(
		CollisionBase::ColliderTag::Haetataki,
		mesh,
		this,
		false
	);

	m_pNowCollider = m_pPickUpCollider;

	CollisionManager::GetInstance()->AddCollider(m_pNowCollider);
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

	//アイテムをプレイヤーの位置に合わせる
	m_vPosition = m_pPlayer->GetPosition() + m_Offset;

	//Nキーで使用状態へ
	if (GetAsyncKeyState('N') & 0x0001)
	{
		m_State = IItemObserver::IItemObserver::State::Use;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Use()
{
	if (m_pNowCollider != m_pUseCollider)
	{
		CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
		m_pNowCollider = m_pUseCollider;
		CollisionManager::GetInstance()->AddCollider(m_pNowCollider);
	}
	//アイテムをプレイヤーの位置に合わせる
	m_vPosition = m_pPlayer->GetPosition() + m_Offset;



	//モーション終了で所持状態へ戻る
	if (!AttackMostion())
	{
		m_State = IItemObserver::IItemObserver::State::Have;
		m_IsMissAttack = false; //初期化

		CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
		m_pNowCollider = m_pPickUpCollider;
		CollisionManager::GetInstance()->AddCollider(m_pNowCollider);
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Throw()
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Destroy()
{
	DestroyItem();
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::ItemState(IItemObserver::State state)
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::TakeMostion()
{
	//目標角度
	constexpr float TARGET_ANGLE_X = 90;
	constexpr float TARGET_ANGLE_Y = 180;

	//所持モーション
	if (m_vRotation.x < D3DXToRadian(TARGET_ANGLE_X))
	{
		m_vRotation.x += m_AddRot.x;
	}
	if (m_vRotation.y < D3DXToRadian(TARGET_ANGLE_Y))
	{
		m_vRotation.y += m_AddRot.y;
	}
}

//--------------------------------------------------------------------------------------------------------------

bool Haetataki::AttackMostion()
{
	//定数宣言
	constexpr float RIGHT_TARGET_POS_X = 0.1f;
	constexpr float LEFT_TARGET_POS_X = 0.2f;


	//使用モーション
	if (m_AddPos.x < RIGHT_TARGET_POS_X && !m_SwitchDir)
	{
		m_vPosition.x += m_AddPos.x;
		m_vRotation.x += m_AddRot.x / 2;	//回転を少し抑える
		m_AddPos.x += ADD_POS_X;
	}
	else
	{
		//trueになると毎回ここに通るので無理やり初期化
		if (!m_SwitchDir)
		{
			m_AddPos = { 0.f, 0.f, 0.f };
		}

		//切り替えしON
		m_SwitchDir = true;
	}

	//切り替えし
	if (m_SwitchDir)
	{
		if (m_AddPos.x < LEFT_TARGET_POS_X)
		{
			m_vPosition.x -= m_AddPos.x;
			m_vRotation.x -= m_AddRot.x / 2; //回転を少し抑える
			m_AddPos.x += ADD_POS_X;
		}
		else
		{
			m_SwitchDir = false;
			m_AddPos = { 0.f,0.f, 0.f };	//初期化

			//モーション終了
			return false;
		}
	}

	//モーション中
	return true;
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayer* player = dynamic_cast<CPlayer*>(other->GetListener()))
		{
			if(m_State == IItemObserver::State::Use)
			Smash(*player);
		}
	}
}

void Haetataki::Smash(CPlayer& playiers)
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

void Haetataki::ChangeCollider()
{
}
