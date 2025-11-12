#include "CShadowManager.h"

CShadowManager::CShadowManager()
	: m_pShadow		()
{
	Create();
}

CShadowManager::~CShadowManager()
{
	Destroy();
}

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
		shadow->SetPosition(0.f, -10.f, 0.f);
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
void CShadowManager::Update(CPlayerManager* players, ItemManager* item)
{
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		auto player = players->GetPlayer(pNo);

		//プレイヤーがいなかったら次へ.
		if (!player) continue;

		//プレイヤーの位置を取得.
		D3DXVECTOR3 playerPos = player->GetPosition();

		if (player->IsAboveGround())
		{
			m_pShadow[pNo]->Update(playerPos);
		}
		else
		{
			m_pShadow[pNo]->SetPosition(0.f, -10.f, 0.f);
		}
	}

	for (int i = Player_Max ; i < item->GetItemVectorNum()+ Player_Max;i++)
	{ 
		m_pShadow[i]->Update(item->GetItemPos(i - Player_Max));
	}

	for (int i = Player_Max + item->GetItemVectorNum();i < Shadow_Max;i++)
	{
		m_pShadow[i]->SetPosition(0, -10.0f, 0);
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