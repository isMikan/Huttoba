#include "CGaugeManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeFrame/CGaugeFrame.h"
#include "GameObject/UIObject/GaugeBase/TimerGauge/CTimerGauge.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerItemAttackState/CPlayerItemAttackState.h"

#include "Item/Items/Bomb/Bomb.h"
#include "Item/Items/Mushroom/Mushroom.h"
#include "Item/Items/TrackingRobot/TrackingRobot.h"

CGaugeManager::CGaugeManager(ItemManager* itemManager)
	: m_pGauge				()

	, m_ObjectGauge			()
	, m_SubscribeObjects	()

	, m_pPlayerManager		()
	, m_pItemManager		( itemManager )

	, m_KnockDownGaugeColor	( 1.f, 0.f, 1.f )
	, m_ItemGaugeColor		( 1.f, 1.f, 0.f )

	, m_DeletePos			( -5.f )
{
	m_pGauge.clear();
	m_pGauge.resize(Gauge_Max);
}

CGaugeManager::~CGaugeManager()
{
	m_SubscribeObjects.clear();
	m_ObjectGauge.clear();
	m_pGauge.clear();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 初期化処理 ---.
void CGaugeManager::Init(
	CPlayerManager* playerManager)
{
	m_pPlayerManager = playerManager;

	//ゲージのインスタンス作成.
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player.get()) continue;	//存在しなかったら次へ.

		auto& bus = player.get()->GetBus();
		//購買処理.
		bus.Subscribe([this, &player](CPlayerState* state) {
			//ダウン状態の場合.
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				GaugeCreate(player.get(), player.get()->GetKnockdownTime(),
					D3DXVECTOR3(m_KnockDownGaugeColor));	//生成.
			}
			//アイテムを持っている場合.
			else if (dynamic_cast<CPlayerHoldingIdleState*>(state))
			{
				auto item = player.get()->GetHoldingItem();
				if (item 
					&& !player.get()->IsAnyHoldingItem<Bomb, Mushroom, TrackingRobot>())
				{
					GaugeCreate(player.get(), item->GetUsageLimit(),
						D3DXVECTOR3(m_ItemGaugeColor));	//生成.
				}
			}
			//アイテムを持っていない場合.
			else if (!player.get()->GetHoldingItem()
				&& !dynamic_cast<CPlayerItemAttackState*>(state))
			{
				Destroy(dynamic_cast<CStaticMeshObject*>(player.get()));	//削除.
			}
		});
	}
}

//--- 更新処理 ---.
void CGaugeManager::Update()
{
	for (auto& item : m_pItemManager->GetItems())
	{
		if (auto mush = dynamic_cast<Mushroom*>(item.get()))
		{
			if (mush->GetIsPlaced())
			{
				GaugeCreate(mush, mush->GetUsageLimit(),
					D3DXVECTOR3(m_ItemGaugeColor));
			}
			else
			{
				Destroy(static_cast<CStaticMeshObject*>(mush));
			}
		}
	}

	bool isDelete = false;	//削除するのか.
	std::vector<CStaticMeshObject*>  deleteObject{};	//削除するオブジェクト.
	int deleteFrame = 0;	//削除するフレーム.
	int deleteGauge = 0;	//削除するゲージ.

	for (auto it = m_ObjectGauge.begin(); it != m_ObjectGauge.end(); it++)
	{
		auto object = it->first;
		auto pair = it->second;	
		int frameNo = pair.first;	//フレーム番号.
		int gaugeNo = pair.second;	//ゲージ番号.

		if (!m_pGauge[frameNo] || !m_pGauge[gaugeNo]) continue;

		//指定した地点に来た場合、削除準備.
		if (object->GetPosition().y < m_DeletePos)
		{
			deleteFrame = frameNo;
			deleteGauge = gaugeNo;
			deleteObject.push_back(object);
			isDelete = true;
			continue;
		}

		//フレーム.
		m_pGauge[frameNo]->SetWorldPos(object->GetPosition());	//世界座標を設定.
		m_pGauge[frameNo]->Update();	//更新.

		//ゲージ.
		m_pGauge[gaugeNo]->SetWorldPos(object->GetPosition());	//世界座標を設定.
		m_pGauge[gaugeNo]->Update();	//更新.

		//タイムゲージクラスの場合.
		if (dynamic_cast<CTimerGauge*>(m_pGauge[gaugeNo].get()))
		{
			if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(object))
			{
				if (player->IsAnyActionState<CPlayerKnockdownState>())
				{
					//時間を取得し、ゲージクラスに渡す.
					m_pGauge[gaugeNo]->SetGaugeInfo(player->GetKnockdownTime());
					m_pGauge[gaugeNo]->SetColor(m_KnockDownGaugeColor);
				}
				else if (player->GetHoldingItem())
				{
					//時間を取得し、ゲージクラスに渡す.
					m_pGauge[gaugeNo]->SetGaugeInfo(player->GetHoldingItem()->GetUsageLimit());

					m_pGauge[gaugeNo]->SetColor(m_ItemGaugeColor);
				}
			}
			else if (Mushroom* mush = dynamic_cast<Mushroom*>(object))
			{
				//時間を取得し、ゲージクラスに渡す.
				m_pGauge[gaugeNo]->SetGaugeInfo(mush->GetUsageLimit());
				std::cout << "ゲージ" << mush->GetUsageLimit().remaining << std::endl;
			}
			else
			{
				deleteFrame = frameNo;
				deleteGauge = gaugeNo;
				deleteObject.push_back(object);
				isDelete = true;
				continue;
			}
		}
	}

	//削除する場合.
	if (isDelete)
	{
		m_pGauge[deleteFrame].reset();
		m_pGauge[deleteGauge].reset();
		for (auto& obj : deleteObject)
		{	
			//プレイヤーを探す.
			auto objectGauge = m_ObjectGauge.find(obj);
			//見つかった場合.
			if (objectGauge != m_ObjectGauge.end())
			{
				m_SubscribeObjects.erase(obj);	//登録を消す.
				m_ObjectGauge.erase(obj);		//ゲージ削除.
			};
		}
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

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- ゲージの構築処理 ---.
void CGaugeManager::GaugeCreate(
	CStaticMeshObject* object, 
	Gauge usageLimit, D3DXVECTOR3 color)
{
	for (int frameNo = 0; frameNo < Gauge_Max; frameNo++)
	{
		if (m_SubscribeObjects.contains(object)) break;
		if (m_pGauge[frameNo]) continue;	//作成されていたら次へ.

		int gaugeNo = frameNo + 1;	//ゲージ番号.

		//ゲージフレームの作成.
		m_pGauge[frameNo] = std::make_unique<CGaugeFrame>();								//インスタンス作成.
		m_pGauge[frameNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));	//スプライト設定.

		//タイムゲージの作成.
		m_pGauge[gaugeNo] = std::make_unique<CTimerGauge>();							//インスタンス作成.
		m_pGauge[gaugeNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));		//スプライト設定.
		m_pGauge[gaugeNo]->SetGaugeInfo(usageLimit);	//時間を設定.
		m_pGauge[gaugeNo]->SetColor(color);				//色を設定.

		m_ObjectGauge[object].first = frameNo;	//フレーム番号を保存.
		m_ObjectGauge[object].second = gaugeNo;	//ゲージ番号を保存.

		m_SubscribeObjects.insert(object);

		break;	//1つだけゲージを作成.
	}
}

//--- 破棄関数 ---.
void CGaugeManager::Destroy(CStaticMeshObject* object)
{
	//オブジェクトを探す.
	auto objectGauge = m_ObjectGauge.find(object);
	//見つかった場合.
	if (objectGauge != m_ObjectGauge.end())
	{
		auto pair = objectGauge->second;
		int frameNo = pair.first;	//フレーム番号.
		int gaugeNo = pair.second;	//ゲージ番号.

		m_pGauge[frameNo].reset();
		m_pGauge[gaugeNo].reset();

		m_SubscribeObjects.erase(object);	//登録を消す.
		m_ObjectGauge.erase(object);		//ゲージ削除.
	}
}