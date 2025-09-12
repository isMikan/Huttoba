#include "stdafx.h"
#include "Haetataki.h"
#include "CCharacter/CPlayer/CPlayer.h"

//--------------------------------------------------------------------------------------------------------------

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Haetataki>("Haetataki"); }

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
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Haetataki));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));

	SetPosition(INITAL_POS_X, INITAL_POS_Y, INITAL_POS_Z);
	SetRotation(INITAL_ROT_X, INITAL_ROT_Y, INITAL_ROT_Z);

	m_State = ItemBase::State::Spawn;
	m_tGravity = INITAL_GRAVITY;
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Update(std::vector<std::unique_ptr<CPlayer>>& playiers)
{
	//アイテム共通のUpdate
	ItemBase::Update(playiers);
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
		m_State = ItemBase::State::OnGround;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::OnGround()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Have;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Have(std::vector<std::unique_ptr<CPlayer>>& playiers)
{
	//アイテムを拾うモーション
	TakeMostion();

	//アイテムをプレイヤーの位置に合わせる
	m_vPosition = playiers[0]->GetPosition() + m_Offset;

	//Nキーで使用状態へ
	if (GetAsyncKeyState('N') & 0x0001)
	{
		m_State = ItemBase::State::Use;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Use(std::vector<std::unique_ptr<CPlayer>>& playiers)
{
	//アイテムをプレイヤーの位置に合わせる
	m_vPosition = playiers[0]->GetPosition() + m_Offset;

	//プレイヤーとの当たり判定
	for (auto& player : playiers)
	{
		if (GetBSphere()->IsHit(*player->GetBSphere()) && playiers[0] != player)
		{
			//当たったときの吹っ飛び座標格納(仮)
			D3DXVECTOR3 newPos = player->GetPosition();
			newPos.x -= 5.f;	

			//敵に当たったときの処理
			player->SetPosition(newPos);
			AssetManager::Sound()->PlaySE(enSoundList::SE_HitHaetataki);
			continue;
		}
		else
		{
			//外れたときのSE(1回だけ鳴るように)
			if (!m_IsMissAttack)
			{
				AssetManager::Sound()->PlaySE(enSoundList::SE_MissHaetataki);
				m_IsMissAttack = true;
			}
			continue;
		}
	}

	//モーション終了で所持状態へ戻る
	if (!AttackMostion())
	{
		m_State = ItemBase::State::Have;
		m_IsMissAttack = false; //初期化
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Throw()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Destroy;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Destroy()
{
	m_IsDestroy = true;
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

