#include "CGaugeManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeFrame/CGaugeFrame.h"
#include "GameObject/UIObject/GaugeBase/TimerGauge/CTimerGauge.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CGaugeManager::CGaugeManager()
	: m_pGauge				()
{
	Create();
}

CGaugeManager::~CGaugeManager()
{
}

void CGaugeManager::Create()
{
	//ゲージのインスタンス作成.
	m_pGauge.clear();
	m_pGauge.resize(Gauge_Max);
	for (auto& gauge : m_pGauge)
	{
		gauge = std::make_unique<CGaugeBase>();
	}
}

void CGaugeManager::LoadData(CPlayerManager* playerManager)
{
	for (int gNo = 0; gNo < Gauge_Max; gNo++)
	{
	//	if (gNo % 2 == 0)
	//	{
			//m_pGauge[gNo] = std::make_unique<CGaugeFrame>();
			//ゲージフレームのスプライトを設定.
			m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));
	//	}
	//	else
	//	{
	//		//ゲージスプライトを設定.
	//		m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));
	//	}

		for (int pNo = 0; pNo < Player_Max; pNo++)
		{
			CPlayerBase* player = playerManager->GetPlayer(pNo);
			if (!player) continue;

			auto& bus = player->GetBus();
			bus.Subscribe([this, player](CPlayerState* state)
				{
					if (dynamic_cast<CPlayerKnockdownState*>(state))
					{
						int id = player->GetPlayerID();
						m_pGauge[static_cast<size_t>(id) + 1] = std::make_unique<CTimerGauge>();
					}
				});

			int gNo = pNo * 2 + 1;
			m_pGauge[gNo]->SubscribePlayerEvent(player);
		}
	}
}

void CGaugeManager::Destroy()
{
}

//--- 更新処理 ---.
void CGaugeManager::Update(CPlayerManager* playerManager)
{
	for (auto& gauge : m_pGauge)
	{
		gauge->Update();

		for (int pNo = 0; pNo < Player_Max; pNo++)
		{
			CPlayerBase* player = playerManager->GetPlayer(pNo);

			if (!player) continue;
			int gNo = pNo * 2;
			m_pGauge[gNo]->SetWorldPos(player->GetPosition());
			
			m_pGauge[static_cast<size_t>(gNo) + 1]->SetGaugeInfo(player->GetKnockdownTime());
			m_pGauge[static_cast<size_t>(gNo) + 1]->SetWorldPos(player->GetPosition());
		}
	}
}

//--- 描画処理 ---.
void CGaugeManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (auto& gauge : m_pGauge)
	{
		gauge->Draw(View, Proj);
	}
}