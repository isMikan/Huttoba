#include "Fun.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Fun>(ItemID::Fun); }

Fun::Fun()
	: m_UseDuration	( 6.0f )		//値を変えるとアイテムの使用可能時間が変化
	, m_HaveOffset	()

	, m_Velocity	()

	, m_Power		( 4.0f )		

	, m_pNowCollider	()
	, m_pUseCollider_2	()
	, m_ThrowHitPlayer	()
{
	Init();

}

Fun::~Fun()
{
	AssetManager::Effect()->Stop(hEffect);
	AssetManager::Sound()->Stop(enSoundList::SE_FunUse);


	CollisionManager::GetInstance()->RemoveCollider(m_pPickUpCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pUseCollider_2.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pNowCollider.get());
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void Fun::Init()
{
	m_UseTime = m_UseDuration;
	m_UsageLimit = { m_UseTime, m_UseDuration };

	AttachMesh(AssetManager::Mesh(StaticMeshList::Fun));

	SetPosition(1, 15, 0);

	m_State = IItemObserver::IItemObserver::State::Spawn;

	m_tGravity = 0.01f;

	//持つ用と攻撃用の当たり判定をそれぞれ用意
	//引数の末尾にfalseを入れると自動登録されなくなり、AddColliderで任意追加できるようにした
	//具体的な使い方はハエたたき見る or 聞く
	
	//使用時の前方に出す当たり判定
	std::shared_ptr<CStaticMesh> UseMesh = AssetManager::Mesh(StaticMeshList::BoomerangCol);
	std::shared_ptr<CStaticMesh> UseMesh_2 = AssetManager::Mesh(StaticMeshList::BCapsule);
	std::shared_ptr<CStaticMesh> PickMesh = AssetManager::Mesh(StaticMeshList::PickUpCol);

	m_pPickUpCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Fan,
		PickMesh,
		this
	);

	m_pUseCollider = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::Fan,
		UseMesh,
		this
	);

	m_pUseCollider_2 = CollisionDataFactory::CreateCapsuleForMesh(
		CollisionBase::ColliderTag::Fan,
		UseMesh_2,
		this
	);

	D3DXVECTOR3 m_UseOffset = { 0.f, 0.5f, 1.5f };
	D3DXVECTOR3 m_UseOffset_2 = { 0.f, 0.5f, 0.5f };
	D3DXVECTOR3 m_PickUpOffset = { 0.f, 0.0f, 0.f };

	m_pUseCollider->SetLocalOffsetToSphere(m_UseOffset);
	m_pUseCollider->SetActive(false);

	m_pUseCollider_2->SetRotationXCapsule(D3DXToRadian(90.f));
	m_pUseCollider_2->SetLocalOffSetToCapsule(m_UseOffset_2, m_UseOffset_2);
	m_pUseCollider_2->SetActive(false);

	m_pPickUpCollider->SetLocalOffsetToSphere(m_PickUpOffset);

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
		m_State = IItemObserver::IItemObserver::State::OnGround;
	}
}

void Fun::OnGround()
{
}

void Fun::Have()
{
	HaveMove();

	AssetManager::Sound()->Stop(enSoundList::SE_FunUse);
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
	
}

void Fun::ItemState(IItemObserver::State state)
{
	AssetManager::Effect()->Stop(hEffect);

	switch (state)
	{
	case IItemObserver::IItemObserver::State::Have:

		m_pPickUpCollider->SetActive(true);
		m_pUseCollider->SetActive(false);
		m_pUseCollider_2->SetActive(false);

		break;
	case IItemObserver::IItemObserver::State::Use:

		m_pPickUpCollider->SetActive(false);
		m_pUseCollider->SetActive(true);
		m_pUseCollider_2->SetActive(true);

		//エフェクト追加
		hEffect = AssetManager::Effect()->Play("FunWind", m_vPosition);

		break;
	case IItemObserver::IItemObserver::State::Throw:
		OneEnterThrow();
		m_pPickUpCollider->SetActive(true);
		m_pUseCollider->SetActive(false);
		m_pUseCollider_2->SetActive(false);

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
			if (m_State == IItemObserver::State::Use && m_pPlayer != player)
			{
				Hit(*player);
				std::cout << player->GetPlayerID() << "と当たった" << std::endl;
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

void Fun::HaveMove()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	//所持中に幹？を持つように見せるのでy座標を調整
	m_vPosition.y -= 0.7;

	m_vQuaternion = m_pPlayer->GetQuaternion();
}

void Fun::UseMove()
{
	m_UseTime -= CTimeManager::GetDeltaTime();
	//ゲージのために追加.	制作者	[甲把]
	m_UsageLimit.remaining = m_UseTime;
	AssetManager::Sound()->PlayLoop(enSoundList::SE_FunUse);


	if (m_UseTime < 0)
	{
		DestroyItem();
	}

	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	//所持中に幹？を持つように見せるのでy座標を調整
	m_vPosition.y -= 0.7;

	m_vQuaternion = m_pPlayer->GetQuaternion();

	//吹き飛ばされてる最中ならアイテムをHave状態にする
	if (m_pPlayer->IsAnyActionState<CPlayerKnockbackState>())
		m_State = IItemObserver::State::Have;


	D3DXVECTOR3 flowerd = m_pPlayer->GetLocalAxes().forward;
	
	float rot = atan2f(flowerd.x, flowerd.z);

	//エフェクトの向きを設定(180をしているのはエフェクトの向きが反対方向だったので追加)
	AssetManager::Effect()->SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(180.f), rot, 0) );
	//エフェクトの再生速度を設定
	AssetManager::Effect()->SetSpeed(hEffect, 1.f);
	//エフェクトの拡縮設定
	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.045f, 0.045f, 0.045f));

	//エフェクトの表示位置
	static D3DXVECTOR3 efectPos;

	efectPos = m_vPosition;
	//手の位置から風を出しているように見えるのでy座標を調整
	efectPos.y += 1.8f;

	AssetManager::Effect()->SetLocation(hEffect, efectPos);
}

void Fun::ThrowMove()
{
	//移動量が一定以下なら
	if (D3DXVec3Length(&m_Velocity) <= 0.6f)
	{
		DestroyItem();
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

	m_Velocity = forward * m_ThrowSpeed;
}

void Fun::Hit(CPlayerBase& playiers)
{
	//プレイヤーの押し出しの計算
	D3DXVECTOR3 SmashVel = playiers.GetPushbackVelocity(m_vPosition, m_Power);

	playiers.SetHitAttack(
		SmashVel,
		CPlayerBase::HitEvent::Pushback);
}