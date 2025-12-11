#include "CShadowManager.h"
#include "Item/ItemBase.h"

CShadowManager::CShadowManager()
	: m_pShadow		()
{
	Create();
}

CShadowManager::~CShadowManager()
{
	Destroy();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築関数 ---.
void CShadowManager::Create()
{
	m_pShadow.clear();
	m_pShadow.resize(Shadow_Max);
	//影クラスのインスタンス作成.
	for (auto& shadow : m_pShadow)
	{
		shadow = std::make_unique<CShadow>();
		shadow->SetScale(0.015f, 0.015f, 1.f);
		shadow->SetPosition(0.f, -10.f, -10.f);
	}
}

//--- 読込関数 ---.
void CShadowManager::LoadData()
{
	//影スプライトを設定.
	for (const auto& shadow : m_pShadow)
	{
		shadow->AttachSprite(AssetManager::Sprite(Sprite3DList::Shadow));
	}
}

//--- 破棄関数 ---.
void CShadowManager::Destroy()
{
	m_pShadow.clear();
}

//--- 更新関数 ---.
void CShadowManager::Update(
	CPlayerManager* players, ItemManager* items)
{
	for (auto& player : players->GetPlayer())
	{

		//プレイヤーがいなかったら次へ.
		if (!player) continue;

		int playerID = player->GetPlayerID();

		//プレイヤーの位置を取得.
		D3DXVECTOR3 playerPos = player->GetPosition();

		if (player->IsAboveGround())
		{
			m_pShadow[playerID]->Update(playerPos);
		}
		else
		{
			m_pShadow[playerID]->SetPosition(0.f, -10.f, -10.f);
		}
	}

	//プレイヤーがすでに影生成をしているため、プレイヤー最大数からスタート
	for (size_t i = Player_Max; i < items->GetItemVectorNum() + Player_Max; i++)
	{
		//for (std::unique_ptr<ItemBase>& item : items->GetItems())
		//{
			if (items->GetItems()[i - Player_Max]->GetIsOnGround())
			{
				m_pShadow[i]->Update(items->GetItemPos(static_cast<int>(i - Player_Max)));
			}
			else
			{
				m_pShadow[i]->SetPosition(0.f, -10.f, -10.f);
			}
		//}
	}
}

//--- 描画処理 ---.
void CShadowManager::Draw(
	CDirectX11* pDx11, D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (auto& shadow : m_pShadow)
	{
		//深度テスト無効にして、影の透過描画をなくす.
		pDx11->SetDepth(false);
		shadow->Draw(View, Proj);
		pDx11->SetDepth(true);
	}
}