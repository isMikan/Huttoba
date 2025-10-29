#include "TrackingRobot.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<TrackingRobot>("TrackingRobot"); }

TrackingRobot::TrackingRobot()
	: m_IsTake			(false)
	, m_PickUpTime		(1.0f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt		(0.0f)

    , m_Velocity		()
	, m_MoveSpeed		( 3.0f )

	, m_IsThrow			( false )

	, m_ExplosionTime	( 5.0f )
	, m_ExplosionCnt	( 0.0f )

    , m_KnockBackPower	( 10.0f )

	, m_ColorTimer		( 0.0f )

    , m_OneExplosion	( false )
{
	Init();
}

TrackingRobot::~TrackingRobot()
{
}

void TrackingRobot::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::TrackingRobot));

	//SetPosition(4, 5, 2);
	SetPosition(0, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;
}

void TrackingRobot::Update()
{
	ItemBase::Update();
}

void TrackingRobot::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void TrackingRobot::Spawn()
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

void TrackingRobot::OnGround()
{
	if (GetAsyncKeyState('7') & 0x0001)
		//if(CInputManager::IsDown(Action::Have,0))
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		//プレイヤー側にあるモーションと同期できるように
		//m_IsTake = true;
	}
}

void TrackingRobot::Have()
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion();
}

void TrackingRobot::Use()
{
	if (GetAsyncKeyState('0') & 0x8000)
	{
		//m_pPlayer->GetVelocity();
	}
}

void TrackingRobot::Throw()
{
}

void TrackingRobot::Destroy()
{
}

void TrackingRobot::TakeMotion()
{
	if (m_IsTake)
	{
		m_PickUpCnt += CTimeManager::GetDeltaTime();

		if (m_PickUpCnt >= m_PickUpTime)
		{
			m_IsTake = false;
		}
	}
}

void TrackingRobot::PossessionMotion()
{
	m_vPosition = m_pPlayer->GetPlayerRightHand().GetPosition();

	if (GetAsyncKeyState('8') & 0x0001)
	{
		m_State = ItemBase::State::Use;

		//投げるときの処理のためにtrueにする
		m_IsThrow = true;
	}
	if (GetAsyncKeyState('9') & 0x8000)
	{
		m_State = ItemBase::State::Throw;

		//投げるときの処理のためにtrueにする
		m_IsThrow = true;
	}
}

void TrackingRobot::UseMotion()
{
}

void TrackingRobot::ThrowMotion()
{
}

void TrackingRobot::Explosion()
{
	if (!m_OneExplosion)
	{
		Smash();
		m_OneExplosion = true;

		static ::EsHandle hEffect = -1;

		hEffect = AssetManager::Effect()->Play("Explosion", m_vPosition);

		//拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

		m_State = ItemBase::State::Destroy;
	}
}

void TrackingRobot::Smash()
{
	D3DXVECTOR3 vecLen = m_vPosition - m_pPlayer->GetPosition();

	float len = D3DXVec3Length(&vecLen);

	//m_pPlayer->SetHitInfo(
	//	m_vPosition, m_pPlayer->GetPosition(),
	//	CalculateForceScalar(len),
	//	true, CPlayerBase::HitEvent::Knockdown);
}

void TrackingRobot::ChangeColor()
{	
	//赤色
	//m_pMesh->SetMaterialColor(0, D3DXVECTOR4(1, 0, 0, 1));
	//黒色(全て0.5が元の色)
	//m_pMesh->SetMaterialColor(0, D3DXVECTOR4(.5f, .5f, .5f, .5f));

	m_ColorTimer += CTimeManager::GetDeltaTime();

	//点滅のスピードを経過時間/爆発するまでの時間をして割合で出す
	double speed = 10.0f * (m_ColorTimer / m_ExplosionTime);

	//+1.0fをすることで、sinの値が0~2の間の値になり、*0.25で0~0.5の値がtに入る
	double blinkRate = (sin(m_ColorTimer * speed) + 1.0) * 0.25;

	//カラー増加変数
	float up = std::clamp(0.5f + static_cast<float>(blinkRate), .5f, 1.0f);

	//カラー減少変数
	float down = std::clamp(0.5f - static_cast<float>(blinkRate), 0.0f, 0.5f);

	//値が増加と減少がそれぞれあるので使いわけていく
	D3DXVECTOR4 color = D3DXVECTOR4(up, down, down, up);

	m_pMesh->SetMaterialColor(0, color);
}

float TrackingRobot::CalculateForceScalar(float distance)
{
	//線形補間で計算

	//爆発の当たる範囲を仮設定
	//当たり判定用メッシュの大きさにする
	float maxDist = 6;

	//0.0~1.0の間で距離の割合を出す
	float ratio = 1.0f - (distance / maxDist);

	//爆発の最小吹き飛ばし力
	float minPower = 6.0f;

	//爆発の最大吹き飛ばし力
	float maxPower = m_KnockBackPower;

	float power = minPower + (maxPower - minPower) * ratio;

	return power;
}
