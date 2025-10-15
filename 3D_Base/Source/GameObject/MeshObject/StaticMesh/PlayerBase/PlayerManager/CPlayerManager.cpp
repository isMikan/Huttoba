#include "CPlayerManager.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
				  
CPlayerManager::CPlayerManager(int index)
	: m_pPlayers	()
	, m_PlayerID	( index )
{
	Create();
}

CPlayerManager::~CPlayerManager()
{
}

//--- 構築関数 ---.
void CPlayerManager::Create()
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
			m_pPlayers = std::make_ptr<CPlayerAI>(pNo);
		}

		if (!m_pPlayers[pNo]) return;

		m_pPlayers[pNo]->SetObjectColor(SetCharacterColor(pNo));
		m_pPlayers[pNo]->SetPosition(SetDefaultPosition(pNo));
	}
}

//--- 読込関数 ---.
void CPlayerManager::LoadData()
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
	}
}

//--- 破棄関数 ---.
void CPlayerManager::Destroy()
{
}

//--- 更新関数 ---.
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

//--- 描画関数 ---.
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

//--- 衝突判定関数 ---.
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

//--- キャラクターの色を設定する関数 ---.
CCharacter::ObjectColor CPlayerManager::SetCharacterColor(int index)
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

//--- 初期位置を設定する関数 ---.
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
