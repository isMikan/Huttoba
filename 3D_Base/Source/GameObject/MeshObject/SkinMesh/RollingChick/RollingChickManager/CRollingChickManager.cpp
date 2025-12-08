#include "CRollingChickManager.h"

CRollingChickManager::CRollingChickManager(
	std::unique_ptr<CPlayerManager>& playerManager)
	: m_pRollingChicks			()

	, m_PlayerChick				()
	, m_SubscribePlayer			()
{
	m_pRollingChicks.clear();
	m_pRollingChicks.resize(Player_Max);

	Create(playerManager);
}

CRollingChickManager::~CRollingChickManager()
{
	m_pRollingChicks.clear();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築関数 ---.
void CRollingChickManager::Create(
	std::unique_ptr<CPlayerManager>& playerManager)
{
	for (auto& player : playerManager->GetPlayer())
	{
		if (!player.get()) continue;	//存在しなかったら次へ.

		auto& bus = player.get()->GetBus();
		//購買処理.
		bus.Subscribe([this, &player](CPlayerState* state) {
			//ダウン状態の場合.
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				for (int cNo = 0;cNo < Player_Max;cNo++)
				{
					if (m_SubscribePlayer.contains(player.get())) break;

					//ひよこクラスのインスタンス作成.
					m_pRollingChicks[cNo] = std::make_unique<CRollingChick>();
					//ひよこクラスのメッシュ読み込み.
					m_pRollingChicks[cNo]->AttachMesh(AssetManager::Mesh(SkinMeshList::Chick));

					//プレイヤーと配列数の照らし合わせ.
					m_PlayerChick[player.get()] = cNo;
					//作成したプレイヤーを保存.
					m_SubscribePlayer.insert(player.get());
				}
			}
			else
			{
				//削除する処理.
				for (auto& chick : m_pRollingChicks)
				{
					chick.reset();
					m_PlayerChick.erase(player.get());
					m_SubscribePlayer.erase(player.get());
				}
			}
		});
	}
}

//--- 更新関数 ---.
void CRollingChickManager::Update()
{
	for (int cNo = 0;cNo < Player_Max;cNo++)
	{
		//なかったら次へ.
		if (!m_pRollingChicks[cNo]) continue;

		for (auto it = m_PlayerChick.begin(); it != m_PlayerChick.end(); it++)
		{
			auto player = it->first;	//プレイヤー.
			auto chickID = it->second;	//ひよこ番号.

			//番号が一致していた場合.
			if(cNo == chickID)
			{
				m_pRollingChicks[cNo]->SetPlayerPos(player);	//プレイヤーの位置に設定.
				m_pRollingChicks[cNo]->Update();				//更新.
			}
		}
	}
}

//--- 描画関数 ---.
void CRollingChickManager::Draw(
	D3DXMATRIX& view, D3DXMATRIX& proj,
	LIGHT& light, CAMERA& camera)
{
	for (auto& chick : m_pRollingChicks)
	{
		//なかったら次へ.
		if (!chick) continue;

		chick->Draw(view, proj, light, camera);
	}
}
