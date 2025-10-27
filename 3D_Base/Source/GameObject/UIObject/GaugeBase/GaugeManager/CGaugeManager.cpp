#include "CGaugeManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeFrame/CGaugeFrame.h"
#include "GameObject/UIObject/GaugeBase/TimerGauge/CTimerGauge.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CGaugeManager::CGaugeManager()
	: m_pGauge				()
{
	m_pGauge.clear();
	m_pGauge.resize(Gauge_Max);
}

CGaugeManager::~CGaugeManager()
{
	Destroy();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築関数 ---.
void CGaugeManager::Create(CPlayerManager* playerManager)
{
	//ゲージのインスタンス作成.
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		CPlayerBase* player = playerManager->GetPlayer(pNo);
		if (!player) continue;	//存在しなかったら次へ.

		auto& bus = player->GetBus();
		//購買処理.
		bus.Subscribe([this, player](CPlayerState* state) {
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				for (int frameNo = 0; frameNo < Gauge_Max; frameNo++)
				{
					if (m_SubscribePlayers.contains(player)) return;

					if (m_pGauge[frameNo]) continue;	//作成されていたら次へ.

					int gaugeNo = frameNo + 1;	//ゲージ番号.

					//ゲージフレームの作成.
					m_pGauge[frameNo] = std::make_unique<CGaugeFrame>();	//インスタンス作成.
					m_pGauge[frameNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));	//スプライト設定.
					std::cout << typeid(m_pGauge[frameNo].get()).name() << std::endl;

					//タイムゲージの作成.
					m_pGauge[gaugeNo] = std::make_unique<CTimerGauge>();	//インスタンス作成.
					m_pGauge[gaugeNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));		//スプライト設定.
					m_pGauge[gaugeNo]->SetGaugeInfo(player->GetKnockdownTime());	//時間を設定.
					std::cout << typeid(m_pGauge[frameNo].get()).name() << std::endl;

					std::cout << "ゲージを作成" << player->GetPlayerID() <<  std::endl;

					m_SubscribePlayers.insert(player);

					m_PlayerGauge[player] = frameNo;	//フレーム番号を保存.

					break;	//1つだけゲージを作成.
				}
			}
			else
			{
				//プレイヤーを探す.
				auto playerGauge = m_PlayerGauge.find(player);
				//見つかった場合.
				if (playerGauge != m_PlayerGauge.end())
				{
					int frameNo = playerGauge->second;	//フレーム番号.
					int gaugeNo = frameNo + 1;			//ゲージ番号.

					m_pGauge[frameNo].reset();
					m_pGauge[gaugeNo].reset();

					m_SubscribePlayers.erase(player);
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
	for (auto& gauge : m_pGauge)
	{
		gauge.reset();
	}
	m_pGauge.clear();
}

//--- 更新処理 ---.
void CGaugeManager::Update(CPlayerManager* playerManager)
{
	for (const auto& pair : m_PlayerGauge)
	{
		CPlayerBase* player = pair.first;	//プレイヤー.
		int frameNo = pair.second;			//フレーム番号.
		int gaugeNo = frameNo + 1;			//ゲージ番号.

		if (!m_pGauge[frameNo] || !m_pGauge[gaugeNo]) continue;

		//タイムゲージクラスの場合.
		if (dynamic_cast<CTimerGauge*>(m_pGauge[gaugeNo].get()))
		{
			//時間を取得し、ゲージクラスに渡す.
			m_pGauge[gaugeNo]->SetGaugeInfo(player->GetKnockdownTime());
			std::cout << "残り時間" << player->GetKnockdownTime().remaining << std::endl;
		}
		//フレーム.
		m_pGauge[frameNo]->Update();	//更新.
		m_pGauge[frameNo]->SetWorldPos(player->GetPosition());	//世界座標を設定.

		//ゲージ.
		m_pGauge[gaugeNo]->Update();	//更新.
		m_pGauge[gaugeNo]->SetWorldPos(player->GetPosition());	//世界座標を設定.
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