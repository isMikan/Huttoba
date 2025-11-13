#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerManager/CPlayerManager.h"

#include "Input/CInputManager.h"
#include "Assets/AssetManager.h"

#include <functional>

class CSceneResult
	: public CScene
{
public:
	CSceneResult();
	~CSceneResult() override;

	HRESULT Create()	override;
	HRESULT LoadData()	override;
	void Update()	override;
	void Draw()		override;
	void Destroy()	override;

private:
	//選択肢の座標を設定.
	void SetSelectorPos();

	void MoveSelector();

	//選択肢がスティックで連続して動いてしまうのを制御する(中間審査用でここに追加したが後でCInputに追加しておく).
	void SelectorControl();
private:
	CDirectX11* m_pDx11;

	std::vector<std::function<void()>>	m_Action;	//画面遷移時の動作を入れる.

	std::unique_ptr<CUIObject>	m_pSpriteResultImg;	//リザルト画面.
	std::unique_ptr<CUIObject>	m_pSpriteSelector;	//選択矢印.

	//プレイヤーマネージャークラス.
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;

	std::vector<D3DXVECTOR3>	m_SelectorPos;		//選択矢印の座標.

	//8ビットの符号付整数型.
	uint8_t						m_SelectorNumber;	//現在選択中の番号.

	//スティックの上下を倒した時の保存.
	bool isHeldUp;
	bool isHeldDown;

	//フレームカウント.
	int cnt;
};
