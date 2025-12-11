#include "CPlayerManager.h"

#include "PlayerBase/Player/CPlayer.h"
#include "PlayerBase/PlayerAI/CPlayerAI.h"
#include "PlayerBase/PlayerAI/PlayerAI_TypeA/CPlayerAI_TypeA.h"
#include "PlayerBase/PlayerAI/PlayerAI_TypeB/CPlayerAI_TypeB.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

#include "Scene/SceneData/CSceneData.h"

CPlayerManager::CPlayerManager()
	: m_pPlayers		()

	, m_InitialSetPosY	()
{
}

CPlayerManager::~CPlayerManager()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		//当たり判定削除.
		CollisionManager::GetInstance()->RemoveCollider(player->GetCollider().get());
	}
	m_pPlayers.clear();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 読込関数 ---.
void CPlayerManager::LoadData()
{
	//プレイヤー.
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.
		
	//=== スタティックメッシュの設定 ===.
		player->AttachMesh(AssetManager::Mesh(StaticMeshList::PBody));	//胴体.
		player->GetPlayerHead().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHead));		//頭.
		player->GetPlayerRightHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));		//右手.
		player->GetPlayerLeftHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));		//左手.
	
		//衝突判定を設定.
		player->CreateCollider();

	//=== 念のためパーツの位置の設定 ===.
		//頭の調整位置を取得.
		D3DXVECTOR3 headOffsetPos = player->GetPlayerHead().GetOffsetPos();
		//頭の位置を設定.
		player->GetPlayerHead().SetPosition(player->GetObjectPos(headOffsetPos));

		//プレイヤーの位置を取得.
		D3DXVECTOR3 playerPos = player->GetPosition();
		//ローカル軸を取得.
		CPlayerBase::LocalAxes axes = player->GetLocalAxes();
		//手の位置を調整して設定.
		player->GetPlayerRightHand().SetPosition(
			player->GetObjectPos(player->GetPlayerRightHand().GetOffsetPos()));
		player->GetPlayerLeftHand().SetPosition(
			player->GetObjectPos(player->GetPlayerLeftHand().GetOffsetPos()));
	}
}

//--- 描画関数 ---.
void CPlayerManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		//描画.
		player->Draw( View, Proj, Light, Camera );						//胴体.
		player->GetPlayerHead().Draw( View, Proj, Light, Camera );		//頭.
		player->GetPlayerRightHand().Draw( View, Proj, Light, Camera );	//右手.
		player->GetPlayerLeftHand().Draw( View, Proj, Light, Camera );	//左手.
	}
}

//=== 各シーンの構築処理 ===.

//--- 準備画面 ---.
void CPlayerManager::StandbyPlayerCreate()
{
	Create();

	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		if (!m_pPlayers[pNo]) return;

		D3DXVECTOR3 pos(2.5f, 0.f, -1.f);
		//左から順に表示.
		pos.x *= pNo;

		m_pPlayers[pNo]->SetPosition(pos);
		m_pPlayers[pNo]->
			SetQuaternion(0.f, D3DXToRadian(180.f), 0.f, 0.f);
	}
}

//--- メイン ---.
void CPlayerManager::MainPlayerCreate(ItemManager* itemManager, CGroundManager* groundManager)
{
	Create();

	for (auto& player : m_pPlayers)
	{
		if (!player) return;

		//プレイヤー番号を取得.
		int id = player->GetPlayerID();

		if(const auto& aiPlayer = dynamic_cast<CPlayerAI*>(player.get()))
		{
			aiPlayer->SetPlayerManager(this);
			aiPlayer->SetItemManager(itemManager);
			aiPlayer->SetGroundManager(groundManager);
		}

		m_InitialSetPosY = 0.f;
		//位置と方向の初期化.
		InitialSettings(id);
	}
}

//--- リザルト ---.
void CPlayerManager::ResultPlayerCreate()
{
	Create();

	int countLive = 0;		//勝者数.
	int countFalled = 0;	//敗者数.
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		D3DXVECTOR3 pos(4.f, 0.f, -1.f);
		//勝利したプレイヤーの場合.
		if (CSceneData::GetPlayerLiving(pNo))
		{
			//右から順に表示.
			pos.x = (pos.x - 2.5f) * (countLive + 1.f);	//配列番号の最後から埋めていく.
			pos.z -= 5.f;

			countLive++;
		}
		//敗北したプレイヤーの場合
		else
		{
			//左から順に表示.
			pos.x = (pos.x * countFalled) - 1.5f;
			pos.y += 6.f;

			countFalled++;
		}

		m_pPlayers[pNo]->SetPosition(pos);
		m_pPlayers[pNo]->
			SetQuaternion(0.f, D3DXToRadian(180.f), 0.f, 0.f);
	}
}

//=== 各シーンの更新処理 ===.

//--- 更新関数 ---.
void CPlayerManager::Update()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.
		//動作.
		player->GetPlayerHead().Update(player->GetQuaternion());	//頭.
		player->GetPlayerRightHand().Update();						//右手.
		player->GetPlayerLeftHand().Update();						//左手.
	}
}

//--- タイトル ---.
void CPlayerManager::TitlePlayerUpdate()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		int id = player->GetPlayerID();
		if (!CSceneData::GetPlayerLiving(id)) continue;	//プレイヤーが生きていない場合、次へ.

		player->Update();	//胴体.

		Update();

		//落ちた場合、死亡判定にする.
		if (player->GetPosition().y < -20.f)
		{
			CSceneData::SetPlayerLive(id, false);

			m_InitialSetPosY = 40.f;
			InitialSettings(id);
		}
	}
}

//--- 準備 ---.
void CPlayerManager::StandbyPlayerUpdate()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		player->Update();	//胴体.

		Update();

		//落ちた場合、削除.
		if (player->GetPosition().x < -10.f)
		{
			Destroy(player.get());
		}
	}
}

//--- メイン ---.
void CPlayerManager::MainPlayerUpdate()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		player->Update();	//胴体.

		Update();

		//落ちた場合、削除.
		if (player->GetPosition().y < -10.f)
		{
			Destroy(player.get());
		}
	}
}

//--- リザルト ---.
void CPlayerManager::ResultPlayerUpdate()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		player->ResultUpdate();
			
		Update();
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

//--- 構築関数 ---.
void CPlayerManager::Create()
{
	//念のため削除.
	m_pPlayers.clear();
	//最大数を設定.
	m_pPlayers.resize(Player_Max);

	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
#if 0
		//プレイヤーのインスタンス生成.
		if (pNo != 1)
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		else
		{
			m_pPlayers[pNo] = std::make_unique<CPlayerAI_TypeB>(pNo);
		}
#else
#if 1
		if (pNo == 0)
		{
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		}
		else
		{
			m_pPlayers[pNo] = std::make_unique<CPlayerAI_TypeA>(pNo);
		}
#else
		//準備OKのコントローラーの場合.
		if (CSceneData::GetSlot(pNo))
		{
			//プレイヤーのインスタンス生成.
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		}
		else
		{
			//プレイヤーAIのインスタンス生成.
			m_pPlayers[pNo] = std::make_unique<CPlayerAI_TypeA>(pNo);
			dynamic_cast<CPlayerAI*>(m_pPlayers[pNo].get())->SetPlayerManager(this);
		}
#endif
#endif

		if (!m_pPlayers[pNo]) return;

		//胴体の色を設定.
		m_pPlayers[pNo]->SetObjectColor(0, CharacterColorSettings(pNo));
		//頭の色を設定.
		m_pPlayers[pNo]->GetPlayerHead().SetObjectColor(1, CharacterColorSettings(pNo));
	}
}

//--- 破棄関数 ---.
void CPlayerManager::Destroy(CPlayerBase* player)
{
	int id = player->GetPlayerID();
	CSceneData::SetPlayerLive(id, false);

	//当たり判定削除.
	CollisionManager::GetInstance()->RemoveCollider(player->GetCollider().get());

	m_pPlayers[id]->SetHoldingItem(nullptr);
	//配列削除.
	m_pPlayers[id].reset();
}

//--- キャラクターの色を設定 ---.
ObjectColor CPlayerManager::CharacterColorSettings(int index)
{
	//プレイヤーの色.
	std::array<ObjectColor, Player_Max>	playerColor =
	{
		ObjectColor
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

//--- 初期位置と方向を設定 ---.
void CPlayerManager::InitialSettings(int index)
{
	//プレイヤーの初期化.
	using InitialSetting = std::map<D3DXVECTOR3, D3DXQUATERNION>;
	static const std::array<InitialSetting, Player_Max> initialSettings =
	{
		InitialSetting
		//プレイヤー1.
		{
			{ D3DXVECTOR3(-6.f, 0.f, 4.f),
				D3DXQUATERNION(0.f, D3DXToRadian(30.f), 0.f, 1.f) }
		},
		//プレイヤー2.
		{
			{ D3DXVECTOR3(6.f, 0.f, 4.f),
				D3DXQUATERNION(0.f, D3DXToRadian(-30.f), 0.f, 1.f) }
		},
		//プレイヤー3.
		{
			{ D3DXVECTOR3(-6.f, 0.f, 15.f),
				D3DXQUATERNION(0.f, D3DXToRadian(120.f), 0.f, 1.f) }
		},
		//プレイヤー4.
		{
			{ D3DXVECTOR3(6.f, 0.f, 15.f),
				D3DXQUATERNION(0.f, D3DXToRadian(-120.f), 0.f, 1.f) }
		}
	};

	for (const auto& setting : initialSettings[index])
	{
		//プレイヤーの位置を設定.
		D3DXVECTOR3 pos = setting.first;
		pos.y += m_InitialSetPosY;
		m_pPlayers[index]->SetPosition(pos);

		//プレイヤーの向きを設定.
		D3DXQUATERNION quat = setting.second;
		D3DXQuaternionNormalize(&quat, &quat);
		m_pPlayers[index]->SetQuaternion(quat);
	}
}