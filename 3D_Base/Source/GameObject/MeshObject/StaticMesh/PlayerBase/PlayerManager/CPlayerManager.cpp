#include "CPlayerManager.h"

#include "PlayerBase/Player/CPlayer.h"
#include "PlayerBase/PlayerAI/CPlayerAI.h"
#include "PlayerBase/PlayerAI/PlayerAI_TypeA/CPlayerAI_TypeA.h"

#include "PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

#include "Scene/SceneData/CSceneData.h"

CPlayerManager::CPlayerManager()
	: m_pPlayers		()

	, m_CreateTime		()
	, m_ReadyTime		( 0.5f )
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

//--- 構築関数 ---.
void CPlayerManager::Create()
{
	//念のため削除.
	m_pPlayers.clear();
	//最大数を設定.
	m_pPlayers.resize(Player_Max);

	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		//準備OKのコントローラーの場合.
		if (CSceneData::GetSlot(pNo))
		{
#if 0
			if (pNo == 0)
			{
				m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
			}
			else
			{
				m_pPlayers[pNo] = std::make_unique<CPlayerAI_TypeA>(pNo);
				dynamic_cast<CPlayerAI*>(m_pPlayers[pNo].get())->SetPlayerManager(this);
			}
#else
			//プレイヤーのインスタンス生成.
			m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
#endif
		}
		else
		{
			//プレイヤーAIのインスタンス生成.
			m_pPlayers[pNo] = std::make_unique<CPlayerAI_TypeA>(pNo);
			dynamic_cast<CPlayerAI*>(m_pPlayers[pNo].get())->SetPlayerManager(this);
		}

		if (!m_pPlayers[pNo]) return;

		//胴体の色を設定.
		m_pPlayers[pNo]->SetObjectColor(0, CharacterColorSettings(pNo));
		//頭の色を設定.
		m_pPlayers[pNo]->GetPlayerHead().SetObjectColor(1, CharacterColorSettings(pNo));
	}
}

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
	//==================================.
	
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
	//==================================.
	}
}

//--- 破棄関数 ---.
void CPlayerManager::Destroy(CPlayerBase* player)
{
	int id = player->GetPlayerID();
	CSceneData::SetPlayerLive(id, false);

	//当たり判定削除.
	CollisionManager::GetInstance()->RemoveCollider(player->GetCollider().get());
	//配列削除.
	m_pPlayers[id].reset();
}

//--- 更新関数 ---.
void CPlayerManager::Update()
{
	for (auto& player : m_pPlayers)
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		float t = CTimeManager::GetTotalTime();
		//開始時すぐには動けないようにする.
		if (t - m_CreateTime > m_ReadyTime)
		{
			player->Update();	//胴体の動作.
		}
		//動作.
		player->GetPlayerHead().Update(player->GetQuaternion());	//頭.
		player->GetPlayerRightHand().Update();						//右手.
		player->GetPlayerLeftHand().Update();						//左手.

		if (player->GetPosition().y < -10.f)
		{
			Destroy(player.get());
		}
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

//--- メインシーンの構築処理 ---.
void CPlayerManager::MainPlayerCreate()
{
	Create();

	for (auto& player : m_pPlayers)
	{
		if (!player) return;

		//プレイヤー番号を取得.
		int id = player->GetPlayerID();

		//位置と方向の初期化.
		InitialSettings(id);
	}

	//生成された時間を取得.
	m_CreateTime = CTimeManager::GetTotalTime();
}

//--- リザルトシーンの構築処理 ---.
void CPlayerManager::ResultPlayerCreate()
{
	Create();

	int countLive = 0;
	int countFalled = 0;
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		D3DXVECTOR3 pos(2.f, 1.f, -3.f);
		if (CSceneData::GetPlayerLiving(pNo))
		{
			countLive++;
			switch (countLive)
			{
			case 1:
				pos.x *= 4.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			case 2:
				pos.x *= 3.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			case 3:
				pos.x *= 2.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			default:
				break;
			}
		}
		else
		{
			countFalled++;
			switch (countFalled)
			{
			case 1:
				pos.x *= 1.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			case 2:
				pos.x *= 2.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			case 3:
				pos.x *= 3.f;
				m_pPlayers[pNo]->SetPosition(pos);
				break;
			default:
				break;
			}
		}
		m_pPlayers[pNo]->
			SetQuaternion(0.f, D3DXToRadian(180.f), 0.f, 0.f);
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
			{ D3DXVECTOR3(-6.f, 0.1f, 4.f),
				D3DXQUATERNION(0.f, D3DXToRadian(30.f), 0.f, 1.f) }
		},
		//プレイヤー2.
		{
			{ D3DXVECTOR3(6.f, 0.1f, 4.f),
				D3DXQUATERNION(0.f, D3DXToRadian(-30.f), 0.f, 1.f) }
		},
		//プレイヤー3.
		{
			{ D3DXVECTOR3(-6.f, 0.1f, 15.f),
				D3DXQUATERNION(0.f, D3DXToRadian(120.f), 0.f, 1.f) }
		},
		//プレイヤー4.
		{
			{ D3DXVECTOR3(6.f, 0.1f, 15.f),
				D3DXQUATERNION(0.f, D3DXToRadian(-120.f), 0.f, 1.f) }
		}
	};

	for (const auto& setting : initialSettings[index])
	{
		//プレイヤーの位置を設定.
		D3DXVECTOR3 pos = setting.first;
		m_pPlayers[index]->SetPosition(pos);

		//プレイヤーの向きを設定.
		D3DXQUATERNION quat = setting.second;
		D3DXQuaternionNormalize(&quat, &quat);
		m_pPlayers[index]->SetQuaternion(quat);
	}
}