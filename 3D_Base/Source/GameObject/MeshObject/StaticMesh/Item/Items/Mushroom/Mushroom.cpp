#include "Mushroom.h"

#include "stdafx.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "TimeManager/CTimeManager.h"

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Mushroom>("Mushroom"); }

Mushroom::Mushroom()
	: m_IsTake		(false)
	, m_PickUpTime	(0.5f)	//時間を変えるとアイテムが手に持つまでの時間が変化
	, m_PickUpCnt	(0.0f)

	, m_HaveOffset	()

	, m_Velocity	()
	, m_MoveSpeed	(6.0)	//値を変えると爆弾の移動相度が変化

	, m_IsThrow		(false)
{
	Init();
}

Mushroom::~Mushroom()
{
}

void Mushroom::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Mushroom));

	SetPosition(3, 15, 0);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;

	m_HaveOffset = D3DXVECTOR3(0.0, 0.2f, 0.0f);
}

void Mushroom::Update(std::unique_ptr<CPlayerManager>& playiers)
{
	ItemBase::Update(playiers);
}

void Mushroom::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Mushroom::Spawn()
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

void Mushroom::OnGround()
{
	if (GetAsyncKeyState('4') & 0x8000)
	{
		//状態を取得中に変化
		m_State = ItemBase::State::Have;

		m_IsTake = true;
	}
}

void Mushroom::Have(std::unique_ptr<CPlayerManager>& playiers)
{
	if (m_IsTake)
		TakeMotion();
	else
		PossessionMotion(playiers);
}

void Mushroom::Use(std::unique_ptr<CPlayerManager>& playiers)
{
	UseAndThrow(playiers);
}

void Mushroom::Throw(std::unique_ptr<CPlayerManager>& playiers)
{
	UseAndThrow(playiers);
}

void Mushroom::Destroy()
{
}

void Mushroom::TakeMotion()
{
	m_PickUpCnt += CTimeManager::GetDeltaTime();

	if (m_PickUpCnt >= m_PickUpTime)
	{
		m_IsTake = false;
	}
}

void Mushroom::PossessionMotion(std::unique_ptr<CPlayerManager>& playiers)
{
	m_vPosition = playiers->GetPlayer(0)->GetPlayerRightHand().GetPosition() + m_HaveOffset;
	m_vQuaternion = playiers->GetPlayer(0)->GetQuaternion();

	if (GetAsyncKeyState('5') & 0x8000)
	{
		m_State = ItemBase::State::Use;
	}
	if (GetAsyncKeyState('6') & 0x8000)
	{
		m_State = ItemBase::State::Throw;
		m_IsThrow = true;
	}
}

void Mushroom::UseMotion()
{
}

void Mushroom::ThrowMotion()
{
}

void Mushroom::UseAndThrow(std::unique_ptr<CPlayerManager>& playiers)
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

	if (GetAsyncKeyState('7') & 0x8000)
	{
		Hit(playiers);
	}
}

void Mushroom::Hit(std::unique_ptr<CPlayerManager>& playiers)
{
	//プレイヤーとキノコのぶつかった方向のベクトル
	D3DXVECTOR3 normal = playiers->GetPlayer(0)->GetPosition() - m_vPosition;

	//ノーマライズして法線ベクトルを取得
	D3DXVec3Normalize(&normal, &normal);

	//プレイヤーの移動方向を取得
	D3DXVECTOR3 velPlayer = playiers->GetPlayer(0)->GetVelocity();

	D3DXVec3Normalize(&velPlayer, &velPlayer);

	//反射方向を計算
	// r = v - 2 * dot(v, n) * n
	D3DXVECTOR3 reflectDir = -2.0f * D3DXVec3Dot(&velPlayer, &normal) * normal;
	D3DXVec3Normalize(&reflectDir, &reflectDir);

	D3DXVECTOR3 a = playiers->GetPlayer(0)->GetPosition() - m_vPosition;

	//ノックバックの強さを計算
	float len = D3DXVec3Length(&a);
	float knockbackPower = CalculateKnockBackPower(len); // ← 距離に応じてパワー計算

	//プレイヤーに吹き飛ばし情報を渡す
	playiers->GetPlayer(0)->SetHitInfo(
		m_vPosition,
		playiers->GetPlayer(0)->GetPosition()/* + reflectDir * 10.0f*/, // 反射方向を位置的に反映
		knockbackPower,
		true,
		CPlayerBase::HitEvent::Knockback);
}

float Mushroom::CalculateKnockBackPower(float distance)
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
	float maxPower = 20;

	float power = minPower + (maxPower - minPower) * ratio;

	return power;
}