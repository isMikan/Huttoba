#include "CGaugeManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeFrame/CGaugeFrame.h"
#include "GameObject/UIObject/GaugeBase/TimerGauge/CTimerGauge.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CGaugeManager::CGaugeManager()
	: m_pGauge				()
{
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
	m_pGauge.clear();
	m_pGauge.resize(Gauge_Max);

	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		CPlayerBase* player = playerManager->GetPlayer(pNo);
		if (!player) continue;	//存在しなかったら次へ.

		auto& bus = player->GetBus();
		bus.Subscribe([this, player](CPlayerState* state) {
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				if (m_SubscribePlayers.contains(player)) return;

				m_SubscribePlayers.insert(player);

				for (int gNo = 0; gNo < Gauge_Max; gNo++)
				{
					if (m_pGauge[gNo]) continue;	//作成されていたら次へ.

					int nextId = gNo + 1;

					//ゲージフレームの作成.
					m_pGauge[gNo] = std::make_unique<CGaugeFrame>();
					m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));
					std::cout << typeid(m_pGauge[gNo].get()).name() << std::endl;

					//ゲージの作成.
					m_pGauge[nextId] = std::make_unique<CTimerGauge>();
					m_pGauge[nextId]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));
					m_pGauge[nextId]->SetGaugeInfo(player->GetKnockdownTime());
					std::cout << typeid(m_pGauge[gNo].get()).name() << std::endl;

					std::cout << "ゲージを作成" << player->GetPlayerID() <<  std::endl;
				}
			}
			else
			{
				//m_SubscribePlayers.erase(player);

				for (int gNo = 0; gNo < Gauge_Max; gNo++)
				{
					if (!m_pGauge[gNo]) continue;	//作成いなかったら次へ.

					int nextId = gNo + 1;
					m_pGauge[gNo].reset();
					m_pGauge[nextId].reset();
				}
			}
		});
	}
}

//--- 読込関数 ---.
void CGaugeManager::LoadData(CPlayerManager* playerManager)
{
	for (int gNo = 0; gNo < Gauge_Max; gNo++)
	{
		if (gNo % 2 == 0)
		{
			m_pGauge[gNo] = std::make_unique<CGaugeFrame>();
			//ゲージフレームのスプライトを設定.
			m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::GaugeFrame));
		}
		else
		{
			//ゲージスプライトを設定.
			m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));
		}
	}
}

//--- 破棄関数 ---.
void CGaugeManager::Destroy()
{
	for (auto& gauge : m_pGauge)
	{
		gauge.reset();
	}
}

//--- 更新処理 ---.
void CGaugeManager::Update(CPlayerManager* playerManager)
{
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		CPlayerBase* player = playerManager->GetPlayer(pNo);

		if (!player) continue;	//存在しなかったら次へ.

		for (int gNo = 0; gNo < Gauge_Max; gNo++)
		{
			if (!m_pGauge[gNo]) continue;

			if (dynamic_cast<CTimerGauge*>(m_pGauge[gNo].get()))
			{
				m_pGauge[gNo]->SetGaugeInfo(player->GetKnockdownTime());
			}
			m_pGauge[gNo]->Update();
			m_pGauge[gNo]->SetWorldPos(player->GetPosition());
			//std::cout << typeid(player).name() << pNo << gNo << std::endl;
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