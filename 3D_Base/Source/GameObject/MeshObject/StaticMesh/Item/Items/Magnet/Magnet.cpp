#include "stdafx.h"
#include "Magnet.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

//--------------------------------------------------------------------------------------------------------------

//Factoryに登録
//namespace { const bool regist = ItemBase::AutoRegister<Magnet>(ItemID::Magnet); }

//--------------------------------------------------------------------------------------------------------------

//定数宣言

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
constexpr float STAGE_HEIGHT = 1.2f;

constexpr float OFFSET_USE_COLLISION_X = 0.0f;
constexpr float OFFSET_USE_COLLISION_Y = 1.1f;
constexpr float OFFSET_USE_COLLISION_Z = 0.0f;

//使用上限
constexpr float USE_COUNT = 7;	

// 回転にかける総時間
constexpr float SLERP_DURATION = 0.5f; 

//回数制限
constexpr float USE_LIMIT = 5;		
//--------------------------------------------------------------------------------------------------------------

Magnet::Magnet()
	: m_Offset			( OFFSET_X, OFFSET_Y, 0.f )
	, m_AddPos			( 0.f, 0.f, 0.f )
	, m_AddRot			( ADD_ROT_X, ADD_ROT_Y, ADD_ROT_Z)
	, m_SwitchDir		( false )
	, m_IsFlyAway		( false )
	, m_IsFlyAwayPower	( 3.f )
	, m_IsMissAttack	( false )
	, m_Velocity		()
	, m_slerpTime		()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------

Magnet::~Magnet()
{
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Init()
{
	m_UseCount = USE_LIMIT;
	m_UsageLimit = { m_UseCount, USE_LIMIT };

	AttachMesh(AssetManager::Mesh(StaticMeshList::Magnet));

	SetPosition(INITAL_POS_X, INITAL_POS_Y, INITAL_POS_Z);

	m_State = IItemObserver::IItemObserver::State::Spawn;
	m_tGravity = INITAL_GRAVITY;


	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Bomb);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Magnet,
		mesh,
		this
	);

	mesh = AssetManager::Mesh(StaticMeshList::BCapsule);

	m_pUseCollider = CollisionDataFactory::CreateCapsuleForMesh(
		CollisionBase::ColliderTag::Magnet,
		mesh,
		this
	);

	//攻撃用判定はいったん使用しない
	m_pUseCollider->SetActive(false);

	D3DXVECTOR3 UseOffset = { OFFSET_USE_COLLISION_X,OFFSET_USE_COLLISION_Y,OFFSET_USE_COLLISION_Z };

	m_pUseCollider->SetLocalOffSetToCapsule(UseOffset, UseOffset);
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Update()
{
	//アイテム共通のUpdate
	ItemBase::Update();
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//アイテム共通のDraw
	ItemBase::Draw(View, Proj, Light, Camera);
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Spawn()
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

void Magnet::OnGround()
{
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Have()
{
	//アイテムを拾うモーション
	TakeMostion();

	m_pPickUpCollider->SetActive(false);
	m_pUseCollider->SetActive(true);

		//位置を合わせる
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	// プレイヤーの回転
	D3DXQUATERNION playerQ = m_pPlayer->GetQuaternion();

#ifdef DEBUG
	static float a = 0.f, b = 0.f, c = 0.f;

	//if (GetKeyState('B') & 0x8000)
	//{
	//	a += 0.5;
	//	std::cout << "Yaw = " << a << std::endl;
	//}
	//if (GetKeyState('N') & 0x8000)
	//{
	//	b += 0.5;
	//	std::cout << "Pitch = " << b << std::endl;
	//}
	//if (GetKeyState('M') & 0x8000)
	//{
	//	c += 0.5;
	//	std::cout << "Roll = " << c << std::endl;
	//}
	//if (GetKeyState('G') & 0x8000)
	//{
	//	a -= 0.5;
	//	std::cout << "Yaw = " << a << std::endl;

	//}
	//if (GetKeyState('H') & 0x8000)
	//{
	//	b -= 0.5;
	//	std::cout << "Pitch = " << b << std::endl;

	//}
	//if (GetKeyState('J') & 0x8000)
	//{
	//	c -= 0.5;
	//	std::cout << "Roll = " << c << std::endl;
	//}

#endif // DEBUG


	// ハエたたきの補正角
	D3DXQUATERNION fix;
	//D3DXQuaternionRotationYawPitchRoll(&fix, D3DXToRadian(a), D3DXToRadian(b), D3DXToRadian(c));
	D3DXQuaternionRotationYawPitchRoll(&fix, D3DXToRadian(90.f), 0, D3DXToRadian(45.f));

	D3DXQUATERNION finalQ;
	D3DXQuaternionMultiply(&finalQ, &fix, &playerQ);
	m_vQuaternion = finalQ;


	m_SwitchDir = false;
	m_AddPos = { 0.f,0.f, 0.f };	//初期化

}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Use()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();


	//モーション終了で所持状態へ戻る
	if (!AttackMostion())
	{
		m_State = IItemObserver::IItemObserver::State::Have;
		m_IsMissAttack = false; //初期化
	}
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Throw()
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

void Magnet::Destroy()
{
	DestroyItem();
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::ItemState(IItemObserver::State state)
{
	switch (state)
	{
	case IItemObserver::State::Have:
		if (m_UseCount <= 0) { Destroy(); }

		break;
	case IItemObserver::State::Use:
		m_UsageLimit.remaining = --m_UseCount;
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

void Magnet::TakeMostion()
{
}

//--------------------------------------------------------------------------------------------------------------

bool Magnet::AttackMostion()
{
	//定数宣言
	constexpr float RIGHT_TARGET_POS_X = 0.1f;
	constexpr float LEFT_TARGET_POS_X = 0.2f;

	// プレイヤーの回転
	D3DXQUATERNION playerQ = m_pPlayer->GetQuaternion();

	// ハエたたきの補正角
	static D3DXQUATERNION Startfix;
	static D3DXQUATERNION Endfix;

	//位置を合わせる
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	//y,x,z
	D3DXQuaternionRotationYawPitchRoll(&Startfix, D3DXToRadian(45.f), D3DXToRadian(45.f), D3DXToRadian(90.f));
	D3DXQuaternionRotationYawPitchRoll(&Endfix, D3DXToRadian(12.f), D3DXToRadian(196.f), D3DXToRadian(81.5));
	//D3DXQuaternionRotationYawPitchRoll(&fix, 0, D3DXToRadian(-45.f), D3DXToRadian(90.f));

	D3DXQUATERNION startRotationQ = playerQ * Startfix;
	D3DXQUATERNION endRotationQ	  = playerQ * Endfix;

	m_slerpTime += (CTimeManager::GetDeltaTime()) * 5;

	float t = m_slerpTime / SLERP_DURATION;
	if (t > 1.0f)
	{
		t = 1.0;
	}

	// 球面線形補間 (Slerp) を実行
	// D3DXQuaternionSlerp 関数で、滑らかに補間された四元数を得る
	D3DXQuaternionSlerp(
		&m_vQuaternion,
		&startRotationQ,
		&endRotationQ,
		t
	);

	//使用モーション
	m_vPosition.x += m_AddPos.x;
	m_AddPos.x += ADD_POS_X;

	// 回転が完了した場合
	if (t >= 1.0f)
	{
		m_slerpTime = 0.0f;
		//std::cout << "モーション終了" << std::endl;
		return false; // モーション終了
	}

	//std::cout << "モーション中" << std::endl;

	//モーション中
	return true;
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayer* player = dynamic_cast<CPlayer*>(other->GetListener()))
		{
			if (m_pPlayer != player)
			{
				if (m_State == IItemObserver::State::Use)
				{
					Smash(*player);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------

void Magnet::Smash(CPlayer& playiers)
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

	static ::EsHandle hEffect = 1;

	//エフェクト追加
	hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

}

//--------------------------------------------------------------------------------------------------------------

void Magnet::OneEnterThrow()
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

	m_Velocity = forward * THROW_SPEED;

	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}