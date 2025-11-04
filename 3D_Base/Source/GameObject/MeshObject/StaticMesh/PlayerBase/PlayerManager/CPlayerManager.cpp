#include "CPlayerManager.h"

#include "PlayerBase/Player/CPlayer.h"
#include "PlayerBase/PlayerAI/CPlayerAI.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

#include "Input//CInputManager.h"



CPlayerManager::CPlayerManager()
	: m_pPlayers	()
{
	Create();
}

CPlayerManager::~CPlayerManager()
{
	Destroy();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築関数 ---.
void CPlayerManager::Create()
{
	//プレイヤーのインスタンス生成.
	m_pPlayers.clear();
	m_pPlayers.resize(Player_Max);
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		//if (CInputManager::IsConnect(pNo))
		{
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		}
		//else
		{
			//m_pPlayers[pNo] = std::make_unique<CPlayerAI>(pNo);
		}

		if (!m_pPlayers[pNo]) return;

		m_pPlayers[pNo]->SetObjectColor(0, SetCharacterColor(pNo));
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

		player->CreateCollider();
	}
}

//--- 破棄関数 ---.
void CPlayerManager::Destroy()
{
	for (auto& player : m_pPlayers)
	{
		//当たり判定削除
		CollisionManager::GetInstance()->RemoveCollider(player->GetCollider().get());
		player.reset();
	}
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

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- キャラクターの色を設定 ---.
CPlayerBase::ObjectColor CPlayerManager::SetCharacterColor(int index)
{
	//プレイヤーの色.
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor =
	{
		CStaticMeshObject::ObjectColor
		//プレイヤー1.
		{
			D3DXVECTOR4(1.f, 0.f, 0.f, 1.f),	
			D3DXVECTOR4(0.5f, 0.f, 0.f, 1.f),	
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),
		},
		//プレイヤー2.
		{
			D3DXVECTOR4(0.f, 0.f, 1.f, 1.f),	
			D3DXVECTOR4(0.f, 0.f, 0.5f, 1.f),	
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f), 
		},
		//プレイヤー3.
		{
			D3DXVECTOR4(1.0f, 0.5f, 0.f, 1.f),		
			D3DXVECTOR4(0.5f, 0.3f, 0.f, 1.f),	
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f), 
		},
		//プレイヤー4.
		{
			D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.f), 
			D3DXVECTOR4(0.0f, 0.5f, 0.0f, 1.f), 
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f), 
		}
	};

	return playerColor[index];
}

//--- 初期位置を設定 ---.
D3DXVECTOR3 CPlayerManager::SetDefaultPosition(int index)
{
	//プレイヤーの位置.
	std::array<D3DXVECTOR3, Player_Max> playerPos =
	{
		D3DXVECTOR3
		//プレイヤー1.
		{ -5.f, 0.f, 5.f },
		//プレイヤー2.
		{ 5.f, 0.f, 5.f },

		//プレイヤー3.
		{ -5.f, 0.f, 10.f },
		//プレイヤー4.
		{ 5.f, 0.f, 10.f }
	};

	return playerPos[index];
}