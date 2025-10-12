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
	, m_PlayerID	( index )
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
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		

		if (m_pInput->IsConnect())
		{
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		}
		else
		{
			m_pPlayers = std::make_ptr<CPlayerAI>(ipNod);
		}

		if (!m_pPlayers[pNo]) return E_POINTER;
	}

	return S_OK;
}

//データの読み込み関数.
HRESULT CPlayerManager::LoadData()
{
	//プレイヤー.
	for (auto& player : m_pPlayers)
	{
		//胴体のスタティックメッシュを設定.
		player->AttachMesh(AssetManager::Mesh(StaticMeshList::PBody));
		//頭のスタティックメッシュを設定.
		player->GetPlayerHead().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHead));
		//右手のスタティックメッシュを設定.
		player->GetPlayerRightHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//左手のスタティックメッシュを設定.
		player->GetPlayerLeftHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//バウンディングスフィアの作成
		player->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));

		player->SetObjectColor(SetCharacterColor(m_PlayerID));
		player->SetPosition(SetDefaultPosition(m_PlayerID));
	}

	return S_OK;
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
		//if (m_pInput->IsRepeat(Action::MoveDown))		z -= 1.f;
		//if (m_pInput->IsRepeat(Action::MoveLeft))		x -= 1.f;
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
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor{};

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
	std::array<D3DXVECTOR3, Player_Max> playerPos{};	//プレイヤーの位置.

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
	//キーボード操作.
	{
		//プレイヤーごとにキーを設定するため.
		using keyMap = std::unordered_map<Action, int>;
		//プレイヤー数分にキーを割り当てる.
		static const std::array<keyMap, Player_Max> keys =
		{
			//プレイヤー1.
			{
				{Action::MoveUp,		VK_UP},		//上移動.
				{Action::MoveDown,		VK_DOWN},	//下移動.
				{Action::MoveLeft,		VK_LEFT},	//左移動.
				{Action::MoveRight,		VK_RIGHT},	//右移動.
				{Action::Attack,		'/'},		//攻撃.
				{Action::ToggleItem,	'-'},		//拾う/捨てる.
			},
			//プレイヤー2.
			{
				{Action::MoveUp,		'W'},	//上移動.
				{Action::MoveDown,		'S'},	//下移動.
				{Action::MoveLeft,		'A'},	//左移動.
				{Action::MoveRight,		'D'},	//右移動.
				{Action::Attack,		'Q'},	//攻撃.
				{Action::ToggleItem,	'E'},	//拾う/捨てる.
			},
			//プレイヤー3.
			{
				{Action::MoveUp,		'T'},	//上移動.
				{Action::MoveDown,		'G'},	//下移動.
				{Action::MoveLeft,		'F'},	//左移動.
				{Action::MoveRight,		'H'},	//右移動.
				{Action::Attack,		'R'}, 	//攻撃.
				{Action::ToggleItem,	'Y'}, 	//拾う/捨てる.
			},
			//プレイヤー4.
			{
				{Action::MoveUp,		'I'},	//上移動.
				{Action::MoveDown,		'K'},	//下移動.
				{Action::MoveLeft,		'J'},	//左移動.
				{Action::MoveRight,		'L'},	//右移動.
				{Action::Attack,		'U'}, 	//攻撃.
				{Action::ToggleItem,	'O'}, 	//拾う/捨てる.
			},
		}

		//プレイヤーにキーを設定.
		for (int pNo = 0; pNo < Player_Max;pNo++)
		{
			for (const auto& [action, key] : keys[pNo])
			{
				CInputManager::Instance().BindKey(
					action, InputBinding(InputDevice::Keyboard, code), pNo);
			}
		}
	}

	//コントローラ操作.
	CInputManager::Instance().BindKey(Action::Attack,		InputBinding(InputDevice::GamePad, CXInput::B));	//攻撃.
	CInputManager::Instance().BindKey(Action::ToggleItem,	InputBinding(InputDevice::GamePad, CXInput::A));	//拾う/捨てる.
}
