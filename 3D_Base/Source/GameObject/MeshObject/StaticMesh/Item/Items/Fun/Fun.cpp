#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>("Fun"); }

Fun::Fun()
	: m_IsTake		(false)
	, m_PickUpTime	(0.5f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt	(0.0f)

	, m_HaveOffset	()

	, m_Velocity	()
	, m_MoveSpeed	( 6.0 )		//値を変えると投げた時の移動速度が変化

	, m_IsThrow		( false )
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

	m_tGravity = 0.01;

	m_HaveOffset = D3DXVECTOR3(0.0, 0.2f, 0.0f);

	std::shared_ptr<CStaticMesh> mesh = AssetManager::Mesh(StaticMeshList::FunCol);

	m_pCollision = CollisionDataFactory::CreateHorizontalCapsule(
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
	if (GetAsyncKeyState('1') & 0x8000)
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		m_IsTake = true;
	}
}

void Fun::Have()
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion();
}

void Fun::Use()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition() + m_HaveOffset;
	m_vQuaternion = m_pPlayer->GetQuaternion();

	static ::EsHandle hEffect = 1;

	//エフェクト追加
	hEffect = AssetManager::Effect()->Play("FunWind", m_vPosition);

	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));
}

void Fun::Throw()
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
	//m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	if (m_vPosition.y > 0.5f)
	{
		m_tGravity += 0.001f;
		m_vPosition.y -= m_tGravity;
		//m_State = State::OnGround;
	}
	else
	{
		m_vPosition.y = 0;
	}

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime()) + m_HaveOffset;
}

void Fun::Destroy()
{
	
}

void Fun::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayer* player = dynamic_cast<CPlayer*>(other->GetListener()))
		{
			//if (m_IsExploded)
			{
				//Hit(*player);
			}
		}
	}
}

void Fun::TakeMotion()
{
	m_PickUpCnt += CTimeManager::GetDeltaTime();

	if (m_PickUpCnt >= m_PickUpTime)
	{
		m_IsTake = false;
	}
}

void Fun::PossessionMotion()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition() + m_HaveOffset;
	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Fun::UseMotion()
{
}

void Fun::ThrowMotion()
{
}

void Fun::Hit(CPlayer& playiers)
{
	//プレイヤーの押し出しの計算
	//D3DXVECTOR3 SmashVel = playiers.GetVelocity(m_vPosition, 2, 10.0f);
	
	//プレイヤーの押し出しの計算
	D3DXVECTOR3 SmashVel = playiers.GetPushbackVelocity(m_vPosition);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Pushback);
}