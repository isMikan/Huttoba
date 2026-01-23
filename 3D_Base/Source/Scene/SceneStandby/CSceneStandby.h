#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"
#include "GameObject/SpriteObject/CSpriteObject.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"

#include "Assets/AssetManager.h"
#include "Input/CInputManager.h"

class CSceneStandby
	: public CScene
{
public:
	CSceneStandby(std::unordered_map<ItemID, bool>& SpawnItemData);
	~CSceneStandby() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	//void InitializePlayers();

	void InitializeRedyFont();

	//選択肢の座標を設定.
	void SetSelectorPos();

	void MoveSelector();

	//アイテムの出現を切り替える
	void SwitchSpawnFlag(ItemID SpawnItemData);
	void SwitchAllSpawnFlag();

private:
	CDirectX11*			m_pDx11;

	//プレイヤーの人数の4がマジックナンバーなので後で変更しておく.
	//準備完了画像.
	std::array < std::unique_ptr<CUIObject>,4 >		m_pRedyFontImg;
	//準備中画像.
	std::array < std::unique_ptr<CUIObject>,4 >		m_pNotRedyFontImg;	

	//アイテムの出現管理UI
	std::unordered_map<ItemID, std::unique_ptr<CUIObject>> m_SelectSpawnItemImg;

	//選択矢印.
	std::unique_ptr<CUIObject>			m_pSpriteSelector;
	std::unique_ptr<CUIObject>			m_pItemSelector;
	std::unique_ptr<CUIObject>			m_pSpriteSelector;
	std::unique_ptr<CUIObject>			m_pReadyHowTo;

	//プレイヤーマネージャークラス.
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;

	//地面マネージャークラス.
	std::unique_ptr<CGroundManager> 	m_pGroundManager;

	std::unique_ptr<CUIObject>	m_pSpriteStartFont;
	std::unique_ptr<CUIObject>	m_pSpriteEndFont;
	
	//画面遷移時の動作を入れる.
	std::vector<std::vector<std::function<void()>>>	m_Action;	

	//8ビットの符号付整数型の現在選択中の番号.
	uint8_t						m_VerticalSelectorNumber;
	uint8_t						m_HorizontalSelectorNumber;
	


	//選択矢印の座標.
	std::vector<std::vector<D3DXVECTOR3>>	m_SelectorPos;

	std::unordered_map<ItemID, bool>& m_SelectSpawnItemData;

	ItemID m_ItemList[7];
};
