#include "stdafx.h"
#include "Bomb.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"
#include "Input/CInputManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Bomb>("Bomb"); }

Bomb::Bomb()
	: m_IsTake			( false )
	, m_PickUpTime		( 0.3f )	//値を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt		( 0.0f )

	, m_Velocity		()
	, m_MoveSpeed		( 3.0f )	//値を変えると爆弾の移動相度が変化

	, m_IsThrow			( false )

	, m_ExplosionTime	( 5.0f )	//値を変えると爆発するまでの時間が変化
	, m_ExplosionCnt	( 0.0f )

	, m_KnockBackPower	( 10.0f )	//値を変えるとプレイヤーの吹き飛ばし力が変化

	, m_ColorTimer		( 0.0 )
{
	Init();
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Bomb));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::Bomb));

	SetPosition(1, 5, 2);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01f;
}

void Bomb::Update(std::unique_ptr<CPlayerManager>& playiers)
{
	ItemBase::Update(playiers);
}

void Bomb::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Bomb::Spawn()
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

void Bomb::OnGround()
{
	if (GetAsyncKeyState('M') & 0x0001)
	//if(CInputManager::IsDown(Action::Have,0))
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		//プレイヤー側にあるモーションと同期できるように
		//m_IsTake = true;
	}
}

void Bomb::Have(std::unique_ptr<CPlayerManager>& playiers)
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion(playiers);
}

void Bomb::Use(std::unique_ptr<CPlayerManager>& playiers)
{
	UseAndThrow(playiers);
}

void Bomb::Throw(std::unique_ptr<CPlayerManager>& playiers)
{
	UseAndThrow(playiers);
}

void Bomb::Destroy()
{
	m_IsDestroy = true;
}

void Bomb::TakeMotion()
{
	m_PickUpCnt += CTimeManager::GetDeltaTime();

	if (m_PickUpCnt >= m_PickUpTime)
	{
		m_IsTake = false;
	}
}

void Bomb::PossessionMotion(std::unique_ptr<CPlayerManager>& playiers)
{
	m_vPosition = playiers->GetPlayer(0)->GetPlayerRightHand().GetPosition();

	if (GetAsyncKeyState('M') & 0x0001)
	{
		m_State = ItemBase::State::Use;
		
		//投げるときの処理のためにtrueにする
		m_IsThrow = true;
	}
	if (GetAsyncKeyState('B') & 0x8000)
	{
		m_State = ItemBase::State::Throw;

		//投げるときの処理のためにtrueにする
		m_IsThrow = true;
	}
}

void Bomb::UseMotion()
{
}

void Bomb::ThrowMotion()
{
}

void Bomb::UseAndThrow(std::unique_ptr<CPlayerManager>& playiers)
{
	if (m_IsThrow)
	{
		//プレイヤーのクォータニオン(向いている方向)記録
		m_vQuaternion = playiers->GetPlayer(0)->GetQuaternion();

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

	m_vPosition += m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	//てきとうに移動速度を減少させている
	m_Velocity -= m_Velocity * static_cast<float>(CTimeManager::GetDeltaTime());

	if (m_vPosition.y > .2f)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += 0.001f;
	}

	ChangeColor();

	m_ExplosionCnt += CTimeManager::GetDeltaTime();

	if (m_ExplosionCnt >= m_ExplosionTime)
	{
		Explosion(playiers);
	}
}

void Bomb::Explosion(std::unique_ptr<CPlayerManager>& playiers)
{
	//一度だけ処理させるために追加
	static bool a = true;

	if (a)
	{
		Blow_Away(playiers);
		a = false;

		static ::EsHandle hEffect = -1;

		hEffect = AssetManager::Effect()->Play("Bomb", m_vPosition);

		//拡縮設定
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.6f, 0.6f, 0.6f));

		m_State = ItemBase::State::Destroy;
	}
}

void Bomb::Blow_Away(std::unique_ptr<CPlayerManager>& playiers)
{
	D3DXVECTOR3 vecLen = m_vPosition - playiers->GetPlayer(0)->GetPosition();

	D3DXVECTOR3 a = D3DXVECTOR3(m_vPosition.x, 0, m_vPosition.z);

	float len = D3DXVec3Length(&vecLen);

	playiers->GetPlayer(0)->SetHitInfo(
		a, playiers->GetPlayer(0)->GetPosition(),
		CalculateKnockBackPower(len),
		true, CPlayerBase::HitEvent::Knockback);
}

void Bomb::ChangeColor()
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

float Bomb::CalculateKnockBackPower(float distance)
{
	//引数が0の時にゼロ除算しないようにするための最小距離の2乗
	float minDistanceSq = 1;

	//吹き飛ばし力を
	float denominator = distance * distance + minDistanceSq;

	return m_KnockBackPower / denominator;
}