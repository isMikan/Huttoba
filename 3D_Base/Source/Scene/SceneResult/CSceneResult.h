#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"
#include "CInput/CInput.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"
#include "Assets/AssetManager.h"

#include <functional>

class CSceneResult
	: public CScene
{
public:
	CSceneResult(CInput& input);
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
private:
	std::vector<std::function<void()>>	m_Action;	//画面遷移時の動作を入れる.

	std::unique_ptr<CUIObject>	m_pSpriteResultImg;	//リザルト画面.
	std::unique_ptr<CUIObject>	m_pSpriteSelector;	//選択矢印.


	//キャラクタークラス
	std::vector< std::unique_ptr<CPlayer> >	m_pPlayer;

	CInput&		m_Input;

	D3DXVECTOR3					m_SelectorPos;		//選択矢印の座標.

	//8ビットの符号付整数型.
	uint8_t						m_SelectorNumber;	//現在選択中の番号.

	std::vector<float>			m_SelectorYPos;		//各選択肢のY座標.
};
