#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>(ItemID::Fun); }

Fun::Fun()
	: m_HaveOffset	()

	, m_Velocity	()
	, m_MoveSpeed	( 6.0f )		//値を変えると投げた時の移動速度が変化

	, m_pNowCollider()
{
	Init();
	
}

Fun::~Fun()
{
	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
}

void Fun::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Fun));

	SetPosition(1, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;

	m_HaveOffset = D3DXVECTOR3(0.0, 0.2f, 0.0f);

	//持つ用と攻撃用の当たり判定をそれぞれ用意
	//引数の末尾にfalseを入れると自動登録されなくなり、AddColliderで任意追加できるようにした
	//具体的な使い方はハエたたき見る or 聞く

	////拾う時の当たり判定
	//std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::Fun);

	//m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
	//	CollisionBase::ColliderTag::Fan,
	//	mesh,
	//	this,
	//	false
	//);

	//使用時の前方に出す当たり判定
	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::FunCol);

	m_pUseCollider = CollisionDataFactory::CreateHorizontalCapsuleForMesh(
		CollisionBase::ColliderTag::Fan,
		mesh,
		this
	);

	SetRotation(D3DXVECTOR3(0.f, D3DXToRadian(-90.f),0.f));
	//m_pNowCollider = m_pUseCollider;

	//ここで現在の当たり判定を設定
	//CollisionManager::GetInstance()->AddCollider(m_pNowCollider);

	//--------------------------------------------------------------------------------------------------------------

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
	if (m_vPosition.y > 0.2)
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

	//投げた瞬間に別のアイテムを持ったり使ったりできるように追加
	//m_pPlayer->SetItemBase(nullptr);
}

void Fun::Destroy()
{
	m_IsDestroy = true;
}

void Fun::ChangeState(State state)
{
	switch (state)
	{
	case ItemBase::State::Spawn:
		break;
	case ItemBase::State::OnGround:
		break;
	case ItemBase::State::Throw:
		OneEnterThrow();
		break;
	case ItemBase::State::Destroy:
		break;
	default:
		break;
	}
}

void Fun::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_State == State::Use)
			{
				if(m_pPlayer!=player)
				{
					Hit(*player);
					//std::cout << player->GetPlayerID() << "と当たった" << std::endl;
				}
			}
			if (m_State == State::Throw && m_pPlayer != player)
			{
				ThrowSmash(*player);
			}
		}
	}
}

void Fun::HaveMove()
{
	//if (m_pNowCollider != m_pPickUpCollider)
	//{
	//	CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
	//	m_pNowCollider = m_pPickUpCollider;
	//	CollisionManager::GetInstance()->AddCollider(m_pNowCollider);
	//}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Fun::UseMove()
{
	//if (m_pNowCollider != m_pUseCollider)
	//{
	//	CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
	//	m_pNowCollider = m_pUseCollider;
	//	CollisionManager::GetInstance()->AddCollider(m_pNowCollider);
	//}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();
	m_vQuaternion = m_pPlayer->GetQuaternion();


	//CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());

	//std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::FunCol);

	//mesh->SetQuaternion(m_vQuaternion);

	//m_pUseCollider = CollisionDataFactory::CreateHorizontalCapsule(
	//	CollisionBase::ColliderTag::Fan,
	//	mesh,
	//	this
	//	//false
	//);
}

void Fun::ThrowMove()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.3f)
	{
		static ::EsHandle hEffect = 1;

		//エフェクト追加
		hEffect = AssetManager::Effect()->Play("Break", m_vPosition);

		//エフェクトの拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.3f, 0.3f, 0.3f));

		m_IsDestroy = true;

		m_pPlayer->SetItemBase(nullptr);
	}

	m_Velocity *= 0.98f;

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());
}

void Fun::OneEnterThrow()
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

	//当たり判定削除
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Fun::Hit(CPlayerBase& playiers)
{
	//プレイヤーの押し出しの計算
	//D3DXVECTOR3 SmashVel = playiers.GetVelocity(m_vPosition, 2, 10.0f);
	
	//プレイヤーの押し出しの計算
	D3DXVECTOR3 SmashVel = playiers.GetPushbackVelocity(m_vPosition, 0.05f);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Pushback);
}