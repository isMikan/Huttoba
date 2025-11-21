#include "CGaugeManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeFrame/CGaugeFrame.h"
#include "GameObject/UIObject/GaugeBase/TimerGauge/CTimerGauge.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

#include "Item/Items/Mushroom/Mushroom.h"

CGaugeManager::CGaugeManager()
	: m_pGauge				()

	, m_pPlayerManager		( nullptr )
	, m_pItemManager		( nullptr )

	, m_PlayerGauge			()
	, m_SubscribePlayers	()

	, m_ItemGauge			()
	, m_SubscribeItems		()
{
	m_pGauge.clear();
	m_pGauge.resize(Gauge_Max);
}

CGaugeManager::~CGaugeManager()
{
	Destroy();

	m_pItemManager = nullptr;
	m_pPlayerManager = nullptr;
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築関数 ---.
void CGaugeManager::Create(CPlayerManager* playerManager, ItemManager* itemManager)
{
	m_pPlayerManager = playerManager;
	m_pItemManager = itemManager;

	//ゲージのインスタンス作成.
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player) continue;	//存在しなかったら次へ.

		auto& bus = player->GetBus();
		//購買処理.
		bus.Subscribe([this, &player](CPlayerState* state) {
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				for (int frameNo = 0; frameNo < Gauge_Max; frameNo++)
				{
					if (m_SubscribePlayers.contains(player.get())) return;

					if (m_pGauge[frameNo]) continue;	//作成されていたら次へ.

					int gaugeNo = frameNo + 1;	//ゲージ番号.

					//ゲージフレームの作成.
					m_pGauge[frameNo] = std::make_unique<CGaugeFrame>();								//インスタンス作成.
					m_pGauge[frameNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));	//スプライト設定.

					//タイムゲージの作成.
					m_pGauge[gaugeNo] = std::make_unique<CTimerGauge>();							//インスタンス作成.
					m_pGauge[gaugeNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));		//スプライト設定.
					m_pGauge[gaugeNo]->SetGaugeInfo(player->GetKnockdownTime());					//時間を設定.

					m_SubscribePlayers.insert(player.get());

					m_PlayerGauge[player.get()] = frameNo;	//フレーム番号を保存.

					break;	//1つだけゲージを作成.
				}
			}
			else if (dynamic_cast<CPlayerHoldingIdleState*>(state))
			{
				for (int frameNo = 0; frameNo < Gauge_Max; frameNo++)
				{
					if (m_SubscribePlayers.contains(player.get())) return;
					if (m_pGauge[frameNo]) continue;	//作成されていたら次へ.

					int gaugeNo = frameNo + 1;	//ゲージ番号.

					//ゲージフレームの作成.
					m_pGauge[frameNo] = std::make_unique<CGaugeFrame>();								//インスタンス作成.
					m_pGauge[frameNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));	//スプライト設定.

					//タイムゲージの作成.
					m_pGauge[gaugeNo] = std::make_unique<CTimerGauge>();							//インスタンス作成.
					m_pGauge[gaugeNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));		//スプライト設定.
					m_pGauge[gaugeNo]->SetGaugeInfo(player->GetItemBase()->GetUsageLimit());		//時間を設定.

					m_SubscribePlayers.insert(player.get());

					m_PlayerGauge[player.get()] = frameNo;	//フレーム番号を保存.

					break;	//1つだけゲージを作成.
				}
			}
			else if (!player->GetItemBase())
			{
				//プレイヤーを探す.
				auto playerGauge = m_PlayerGauge.find(player.get());
				//見つかった場合.
				if (playerGauge != m_PlayerGauge.end())
				{
					int frameNo = playerGauge->second;	//フレーム番号.
					int gaugeNo = frameNo + 1;			//ゲージ番号.

					m_pGauge[frameNo].reset();
					m_pGauge[gaugeNo].reset();

					m_SubscribePlayers.erase(player.get());
					m_PlayerGauge.erase(playerGauge);
				}
			}
		});

	}
}

//--- 破棄関数 ---.
void CGaugeManager::Destroy()
{
	m_SubscribePlayers.clear();
	m_PlayerGauge.clear();
	m_pGauge.clear();
}

//--- 更新処理 ---.
void CGaugeManager::Update()
{
	for (auto& [player, frameNo] : m_PlayerGauge)
	{
		int gaugeNo = frameNo + 1;			//ゲージ番号.

		if (!m_pGauge[frameNo] || !m_pGauge[gaugeNo]) continue;

		//タイムゲージクラスの場合.
		if (dynamic_cast<CTimerGauge*>(m_pGauge[gaugeNo].get()))
		{
			if (player->IsAnyActionState<CPlayerKnockdownState>())
			{
				//時間を取得し、ゲージクラスに渡す.
				m_pGauge[gaugeNo]->SetGaugeInfo(player->GetKnockdownTime());
			}
			else if (player->GetItemBase())
			{
				//時間を取得し、ゲージクラスに渡す.
				m_pGauge[gaugeNo]->SetGaugeInfo(player->GetItemBase()->GetUsageLimit());
			}
		}
		//フレーム.
		m_pGauge[frameNo]->Update();	//更新.
		m_pGauge[frameNo]->SetWorldPos(player->GetPosition());	//世界座標を設定.

		//ゲージ.
		m_pGauge[gaugeNo]->Update();	//更新.
		m_pGauge[gaugeNo]->SetWorldPos(player->GetPosition());	//世界座標を設定.
	}

	for (auto& [item, frameNo] : m_ItemGauge)
	{
		int gaugeNo = frameNo + 1;			//ゲージ番号.

		if (!m_pGauge[frameNo] || !m_pGauge[gaugeNo]) continue;

		//タイムゲージクラスの場合.
		if (dynamic_cast<CTimerGauge*>(m_pGauge[gaugeNo].get()))
		{
			if (item)
			{
				//時間を取得し、ゲージクラスに渡す.
				m_pGauge[gaugeNo]->SetGaugeInfo(item->GetUsageLimit());
			}
		}
		//フレーム.
		m_pGauge[frameNo]->Update();	//更新.
		m_pGauge[frameNo]->SetWorldPos(item->GetPosition());	//世界座標を設定.

		//ゲージ.
		m_pGauge[gaugeNo]->Update();	//更新.
		m_pGauge[gaugeNo]->SetWorldPos(item->GetPosition());	//世界座標を設定.
	}
}

//--- 描画処理 ---.
void CGaugeManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (auto& gauge : m_pGauge)
	{
		if (!gauge) continue;	//存在しなかったら次へ.

		gauge->Draw(View, Proj);
	}
}

void CGaugeManager::ItemState(IItemObserver::State state)
{
	for (auto& item : m_pItemManager->GetItems())
	{
		if (Mushroom* mush = dynamic_cast<Mushroom*>(item.get()))
		{
			if (state == IItemObserver::State::Use)
			{
				for (int frameNo = Player_Max; frameNo < Gauge_Max; frameNo++)
				{
					if (m_SubscribeItems.contains(item.get())) return;
					if (m_pGauge[frameNo]) continue;	//作成されていたら次へ.

					int gaugeNo = frameNo + 1;	//ゲージ番号.

					//ゲージフレームの作成.
					m_pGauge[frameNo] = std::make_unique<CGaugeFrame>();								//インスタンス作成.
					m_pGauge[frameNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));	//スプライト設定.

					//タイムゲージの作成.
					m_pGauge[gaugeNo] = std::make_unique<CTimerGauge>();						//インスタンス作成.
					m_pGauge[gaugeNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));	//スプライト設定.
					m_pGauge[gaugeNo]->SetGaugeInfo(item->GetUsageLimit());	//時間を設定.

					m_SubscribeItems.insert(item.get());

					m_ItemGauge[item.get()] = frameNo;	//フレーム番号を保存.

					break;	//1つだけゲージを作成.
				}
			}
		}
		else if (item)
		{
			//プレイヤーを探す.
			auto itemGauge = m_ItemGauge.find(item.get());
			//見つかった場合.
			if (itemGauge != m_ItemGauge.end())
			{
				int frameNo = itemGauge->second;	//フレーム番号.
				int gaugeNo = frameNo + 1;			//ゲージ番号.

				m_pGauge[frameNo].reset();
				m_pGauge[gaugeNo].reset();

				m_SubscribeItems.erase(item.get());
				m_ItemGauge.erase(itemGauge);
			}
		}
	}
}