#include "CShadowManager.h"

CShadowManager::CShadowManager()
	: m_pShadow()
{
	Create();
}

CShadowManager::~CShadowManager()
{
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
	}
}

//--- 読込関数 ---.
void CShadowManager::LoadData()
{
	//影スプライトを設定.
	for (const auto& shadow : m_pShadow)
	{
		shadow->AttachSprite(AssetManager::Sprite(Sprite3DList::Explosion));
	}
}

//--- 破棄関数 ---.
void CShadowManager::Destroy()
{
	m_pShadow.clear();
}

//--- 更新関数 ---.
void CShadowManager::Update(CPlayerManager* player, ItemManager* item)
{
	for (int pNo = 0; pNo < Player_Max; pNo++)
	{
		if (!player->GetPlayer(pNo)) continue;
		m_pShadow[pNo]->Update(player->GetPlayer(pNo)->GetPosition());
	}
}

//--- 描画処理 ---.
void CShadowManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (auto& shadow : m_pShadow)
	{
		shadow->Draw(View, Proj);
	}
}