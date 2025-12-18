#pragma once
#include "Scene/CScene.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Ground/GroundManager/CGroundManager.h"
#include "GameObject/UIObject/Sky/CSky.h"

#include "GameObject/UIObject/CUIObject.h"
#include "Assets/AssetManager.h"
#include "Input/CInputManager.h"

class CSceneTitle
	: public CScene
{
public:
	CSceneTitle(HWND hWnd);
	~CSceneTitle() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	//選択肢の座標を設定.
	void SetSelectorPos();

	void MoveSelector();

private:
	CDirectX11*			m_pDx11;
	HWND				m_hWnd;			//ウィンドウハンドル

	//画面遷移時の動作を入れる
	std::vector<std::function<void()>>	m_Action;	

	//プレイヤーマネージャークラス.
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;

	//地面マネージャークラス.
	std::unique_ptr<CGroundManager> 	m_pGroundManager;
	
	//タイトル
	std::unique_ptr<CUIObject>	m_pSpriteTitleImg;	

	//選択矢印
	std::unique_ptr<CUIObject>	m_pSpriteSelector;	

	//スタートフォント
	std::unique_ptr<CUIObject>	m_pSpriteStartFont;
	//エンドフォント
	std::unique_ptr<CUIObject>	m_pSpriteEndFont;

	//選択矢印の座標
	std::vector<D3DXVECTOR3>	m_SelectorPos;		

	std::unique_ptr<ItemManager> m_pItemManager;

	//現在選択中の番号
	uint8_t						m_SelectorNumber;	

	float m_StartTime;
	float m_EndTime;

	//中間までに導入する時間がないので時間があれば追加できるようにする
	//float						m_SthikThreshold;	//選択肢を動かすときのスティックの最小値.
};
