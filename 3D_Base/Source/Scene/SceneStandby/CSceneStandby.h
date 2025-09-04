#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"

#include "CInput/CInputManager.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"
#include "Camera/CCamera.h"

#include "Assets/AssetManager.h"

class CSceneStandby
	: public CScene
{
public:
	CSceneStandby(CInputManager& input);
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
private:
	std::unique_ptr<CUIObject>	m_pSpriteStandbyImg;	//待機画面.

	//プレイヤーの人数の4がマジックナンバーなので後で変更しておく.
	std::array < std::unique_ptr<CUIObject>,4 >		m_pRedyFontImg;		//準備完了画像.
	std::array < std::unique_ptr<CUIObject>,4 >		m_pNotRedyFontImg;	//準備中画像.

	std::unique_ptr<CUIObject>	m_pSpriteSelector;	//選択矢印.

	std::unique_ptr<CCamera>	m_pCamera;	//カメラ.

	//キャラクタークラス
	std::array< std::unique_ptr<CPlayer>,4 >		m_pPlayer;

	CInputManager&									m_InputManager;

	std::vector<std::function<void()>>	m_Action;	//画面遷移時の動作を入れる.

	D3DXVECTOR3					m_SelectorPos;		//選択矢印の座標.

	//8ビットの符号付整数型.
	uint8_t						m_SelectorNumber;	//現在選択中の番号.

	std::vector<float>			m_SelectorYPos;		//各選択肢のY座標.
};
