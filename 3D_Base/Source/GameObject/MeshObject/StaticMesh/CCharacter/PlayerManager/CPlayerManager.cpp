#include "CPlayerManager.h"

#include "CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerMoveState/PlayerIdle/CPlayerMoveIdle.h"
#include "CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerMoveState/PlayerIdle/CPlayerMoveIdle.h"
#include "CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerRotationState/PlayerRotationIdle/CPlayerRotationIdle.h"
#include "CCharactor/CPlayer/PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"

#include "CCharactor/CPlayer/PlayerState/ActionState/PlayerHandAttack/CPlayerHandAttack.h"
#include "CCharactor/CPlayer/PlayerState/ActionState/PlayerPickupItem/CPlayerPickupItem.h"
#include "CCharactor/CPlayer/PlayerState/ActionState/PlayerThrowItem/CPlayerThrowItem.h"
#include "CCharactor/CPlayer/PlayerState/ActionState/PlayerPushed/CPlayerPushed.h"

CPlayerManager::CPlayerManager(int index)
	: m_pPlayers	()
	, m_PlayerID	(index)
{
	Create();
}

CPlayerManager::~CPlayerManager()
{
}

//構築関数.
HRESULT CPlayerManager::Create()
{
	//プレイヤーのインスタンス生成.
	m_pPlayers.resize(Player_Max);
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		if (!m_pPlayers[pNo]) return E_POINTER;
	}

	return S_OK;
}

//データの読み込み関数.
HRESULT CPlayerManager::LoadData()
{
	//プレイヤー.
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		//胴体のスタティックメッシュを設定.
		m_pPlayers[pNo]->AttachMesh(AssetManager::Mesh(StaticMeshList::PBody));
		//頭のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerHead().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHead));
		//右手のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerRightHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//左手のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerLeftHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//バウンディングスフィアの作成
		m_pPlayers[pNo]->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));
	}

	return S_OK;
}

//初期化関数.
void CPlayerManager::Init()
{
	for (auto& player : m_pPlayers)
	{
		player->SetObjectColor(SetCharacterColor(m_PlayerID));
		player->SetPosition(SetDefaultPosition(m_PlayerID));
	}
}

//破棄関数.
void CPlayerManager::Destroy()
{
}

//更新関数.
void CPlayerManager::Update()
{
	for (auto& player : m_pPlayers)
	{
		//動作.
		player->Update();						//胴体.
		player->GetPlayerHead().Update();		//頭.
		player->GetPlayerRightHand().Update();	//右手.
		player->GetPlayerLeftHand().Update();	//左手.
	}
}

//描画関数.
void CPlayerManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (auto& player : m_pPlayers)
	{
		//描画.
		player->Draw( View, Proj, Light, Camera );						//胴体.
		player->GetPlayerHead().Draw( View, Proj, Light, Camera );		//頭.
		player->GetPlayerRightHand().Draw( View, Proj, Light, Camera );	//右手.
		player->GetPlayerLeftHand().Draw( View, Proj, Light, Camera );	//左手.

		//当たり判定の中心座標を更新する
		player->UpdateBSpherePos();
	}
}

void CPlayerManager::Collision()
{
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		for (int aNo = 0;aNo < Player_Max;aNo++)
		{
			if (pNo == aNo) continue;

			if (m_pPlayers[aNo]->IsAttacking()
				&& m_pPlayers[aNo]->GetBSphere()->
				IsHit(*m_pPlayers[pNo]->GetBSphere()))
			{
				m_pPlayers[pNo]->SetHitInfo(
					m_pPlayers[aNo]->GetPosition(), 0.05f, true);

				m_pPlayers[aNo]->SetHitInfo(
					m_pPlayers[aNo]->GetPosition(), 0.f, true);
			}
		}
	}
}

void CPlayerManager::HandleInput()
{
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		//float x = 0.f;	//x軸.
		//float z = 0.f;	//z軸.

		//if (m_pInput->IsRepeat(Action::MoveUp))		z += 1.f;
		//if (m_pInput->IsRepeat(Action::MoveDown))	z -= 1.f;
		//if (m_pInput->IsRepeat(Action::MoveLeft))	x -= 1.f;
		//if (m_pInput->IsRepeat(Action::MoveRight))	x += 1.f;

		//接続されていたら数値を受け取る.
		//if (m_pInput->IsConnect())
		//{
		//	x = m_pInput->GetLeftSthikX();
		//	z = m_pInput->GetLeftSthikY();
		//}

		//m_pMoveState->KeyInput(*this, x, z);
		//m_pRotationState->KeyInput(*this, x, z);

		//アイテムを持っていないなら攻撃.
		if (CInputManager::Instance().GetInput(pNo).IsDown(Action::Attack)
			&& !m_pPlayers[pNo]->IsHoldingItem()
			&& !m_pPlayers[pNo]->IsAttacking())
		{
			m_pPlayers[pNo]->SetActionState(std::make_unique<CPlayerHandAttack>());
		}
		//アイテムを持っていないなら拾う.
		if (CInputManager::Instance().GetInput(pNo).IsDown(Action::ToggleItem)
			&& !m_pPlayers[pNo]->IsHoldingItem())
		{
			m_pPlayers[pNo]->SetActionState(std::make_unique<CPlayerPickupItem>());
		}
		//アイテムを持っているなら捨てる.
		else if (CInputManager::Instance().GetInput(pNo).
			IsDown(Action::ToggleItem)
			&& m_pPlayers[pNo]->IsHoldingItem())
		{
			m_pPlayers[pNo]->SetActionState(std::make_unique<CPlayerThrowItem>());
		}

		if (m_pPlayers[pNo]->GetHitInfo().isHit == true
			&& m_pPlayers[pNo]->GetHitInfo().force > 0.f)
		{
			m_pPlayers[pNo]->SetActionState(std::make_unique<CPlayerPushed>());
		}
	}

}

//エフェクトを表示するための関数.
//void CPlayerManager::ManageEffectLaser(static::EsHandle hEffect)
//{
//	if (GetAsyncKeyState('Y') & 0x0001)
//	{
//		for (auto& player : m_pPlayers)
//		{
//			hEffect = AssetManager::Effect()->Play("Laser", player->GetPosition());
//
//			//拡縮
//			AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
//			AssetManager::Effect()->SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(-90.f), 0.f, 0.f));
//			AssetManager::Effect()->SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
//		}
//	}
//
//	//Effect制御
//	if (GetAsyncKeyState('T') & 0x0001)
//	{
//		AssetManager::Effect()->Stop(hEffect);
//	}
//}

//キャラクターの色を設定する関数.
CPlayer::ObjectColor CPlayerManager::SetCharacterColor(int index)
{
	//プレイヤーの色.
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor;

	switch (index)
	{
	case 0:
		playerColor[index] = {
			D3DXVECTOR4(1.f, 0.f, 0.f, 1.f), // 赤
			D3DXVECTOR4(0.5f, 0.f, 0.f, 1.f), // 少し暗めの赤
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),
		};

		break;
	case 1:
		playerColor[index] = {
			D3DXVECTOR4(0.f, 0.f, 1.f, 1.f),  // 青
			D3DXVECTOR4(0.f, 0.f, 0.5f, 1.f),  // 少し暗めの青
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 2:
		playerColor[index] = {
			D3DXVECTOR4(1.0f, 0.5f, 0.f, 1.f),  // オレンジ
			D3DXVECTOR4(0.5f, 0.3f, 0.f, 1.f), // 少し暗めのオレンジ
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 3:
		playerColor[index] = {
			D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.f),  // 緑
			D3DXVECTOR4(0.0f, 0.5f, 0.0f, 1.f),  // 少し暗めの緑
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	default:
		break;
	}
	return playerColor[index];
}

//初期位置を設定する関数.
D3DXVECTOR3 CPlayerManager::SetDefaultPosition(int index)
{
	std::array<D3DXVECTOR3, Player_Max> playerPos;	//プレイヤーの位置.

	switch (index)
	{
	case 0:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 5.f);

		break;
	case 1:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 5.f);

		break;
	case 2:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 10.f);

		break;
	case 3:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 10.f);

		break;
	default:
		break;
	}

	return playerPos[index];
}

//キーバインドを設定する関数.
void CPlayerManager::SetPlayerInputBinding()
{
	std::array<std::unordered_map<Action, int>,Player_Max> keys;

	for (int pNo = 0; pNo < Player_Max;pNo++)
	{
		switch (pNo)
		{
		case 0:
			keys[pNo] =
			{
				{Action::MoveUp,		VK_UP},
				{Action::MoveDown,		VK_DOWN},
				{Action::MoveLeft,		VK_LEFT},
				{Action::MoveRight,		VK_RIGHT},
				{Action::Attack,		'Z'},
				{Action::ToggleItem,	'X'},
			};

			break;
		case 1:
			keys[pNo] =
			{
				{Action::MoveUp,		'W'},
				{Action::MoveDown,		'S'},
				{Action::MoveLeft,		'A'},
				{Action::MoveRight,		'D'},
				{Action::Attack,		'Q'},
				{Action::ToggleItem,	'E'},
			};

			break;
		default:
			break;
		}

		for(auto& key : keys[pNo])
		{
			Action action = key.first;
			int code = key.second;

			CInputManager::Instance().BindKey(
				action, InputBinding(InputDevice::Keyboard, code), pNo);
		}
	}

	////キーボード操作.
	//CInputManager::Instance().BindKey(Action::MoveUp,		InputBinding(InputDevice::Keyboard, VK_UP), 0);		//上移動.
	//CInputManager::Instance().BindKey(Action::MoveDown,		InputBinding(InputDevice::Keyboard, VK_DOWN), 0);		//下移動.
	//CInputManager::Instance().BindKey(Action::MoveLeft,		InputBinding(InputDevice::Keyboard, VK_LEFT), 0);		//左移動.
	//CInputManager::Instance().BindKey(Action::MoveRight,	InputBinding(InputDevice::Keyboard, VK_RIGHT), 0);		//右移動.
	//CInputManager::Instance().BindKey(Action::Attack,		InputBinding(InputDevice::Keyboard, 'Z'), 0);			//攻撃.
	//CInputManager::Instance().BindKey(Action::ToggleItem,	InputBinding(InputDevice::Keyboard, 'X'), 0);			//拾う/捨てる.

	//コントローラ操作.
	CInputManager::Instance().BindKey(Action::Attack,		InputBinding(InputDevice::GamePad, CXInput::B));	//攻撃.
	CInputManager::Instance().BindKey(Action::ToggleItem,	InputBinding(InputDevice::GamePad, CXInput::A));	//拾う/捨てる.
}
