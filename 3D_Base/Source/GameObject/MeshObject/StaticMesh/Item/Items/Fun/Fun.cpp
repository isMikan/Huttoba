#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>("Fun"); }

Fun::Fun()
	: m_IsUse		( false )

	, m_HaveOffset	()

	, m_Velocity	()
	, m_MoveSpeed	( 6.0 )		//値を変えると投げた時の移動速度が変化

	, m_IsThrow		( true )
{
	Init();
}

Fun::~Fun()
{
}

void Fun::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Fun));

	SetPosition(1, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;

	m_HaveOffset = D3DXVECTOR3(0.0, 0.2f, 0.0f);

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Fun);

	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		this,
		mesh,
		CollisionBase::ColliderTag::Bomb
	);
}

void Fun::Update()
{
	ItemBase::Update();
}

void Fun::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Fun::Spawn()
{
	//落下処理
	if (m_vPosition.y > 1.2)
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

void Fun::OnGround()
{
}

void Fun::Have()
{
	HaveMove();
}

void Fun::Use()
{
	UseMove();
}

void Fun::Throw()
{
	ThrowMove();
}

void Fun::Destroy()
{
	m_IsDestroy = true;
}

void Fun::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_IsUse)
			{
				if(m_pPlayer!=player)
				{
					Hit(*player);
					std::cout << player->GetPlayerID() << "と当たった" << std::endl;
				}
			}
		}
	}
}

void Fun::HaveMove()
{
	if (m_IsUse)
	{
		//当たり判定削除
		//CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

		std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Fun);

		m_pCollision = CollisionDataFactory::CreateSphereForMesh(
			this,
			mesh,
			CollisionBase::ColliderTag::Bomb
		);

		//m_IsUse = false;
	std::cout << "持っているに状態変化" << std::endl;
	}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Fun::UseMove()
{
	if (!m_IsUse)
	{
		//当たり判定削除
		CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

		std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::FunCol);

		m_pCollision = CollisionDataFactory::CreateHorizontalCapsule(
			this,
			mesh,
			CollisionBase::ColliderTag::Bomb
		);

		m_IsUse = true;
		std::cout << "使ってる状態に変化" << std::endl;
	}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Fun::ThrowMove()
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
		//CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

		std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Fun);

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
	}

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void Fun::Hit(CPlayerBase& playiers)
{
	//プレイヤーの押し出しの計算
	//D3DXVECTOR3 SmashVel = playiers.GetVelocity(m_vPosition, 2, 10.0f);
	
	//プレイヤーの押し出しの計算
	D3DXVECTOR3 SmashVel = playiers.GetPushbackVelocity(m_vPosition);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Pushback);
}