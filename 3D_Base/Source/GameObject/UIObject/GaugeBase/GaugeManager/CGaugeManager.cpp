#include "CGaugeManager.h"

CGaugeManager::CGaugeManager()
	: m_pGauge				()

	, m_IsSubscribe			( false )
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
	m_pGauge.resize(Player_Max);
	for (int gNo = 0; gNo < Player_Max; gNo++)
	{
		m_pGauge[gNo] = std::make_unique<CGaugeBase>();
	}
}

void CGaugeManager::LoadData()
{
	for (int gNo = 0; gNo < Player_Max; gNo++)
	{
		//スプライトを設定.
		m_pGauge[gNo]->AttachSprite(AssetManager::Sprite(Sprite2DList::Gauge));
	}
}

void CGaugeManager::Destroy()
{
}

//--- 更新処理 ---.
void CGaugeManager::Update(CPlayerManager* playerManager)
{
	for (int gNo = 0; gNo < Player_Max; gNo++)
	{
		m_pGauge[gNo]->Update();

		if (!m_IsSubscribe)
		{
			for (int pNo = 0; pNo < Player_Max; pNo++)
			{
				CPlayerBase* player = playerManager->GetPlayer(pNo);
				if (!player) continue;
				m_pGauge[pNo]->SubscribePlayerEvent(player);
			}
			m_IsSubscribe = true;
		}

		for (int pNo = 0; pNo < Player_Max; pNo++)
		{
			CPlayerBase* player = playerManager->GetPlayer(pNo);
			if (!player) continue;
			m_pGauge[pNo]->SetWorldPos(player->GetPosition());
		}
	}
}

//--- 描画処理 ---.
void CGaugeManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (int gNo = 0; gNo < Player_Max; gNo++)
	{
		m_pGauge[gNo]->Draw(View, Proj);
	}
}