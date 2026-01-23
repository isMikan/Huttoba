#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Ground/GroundManager/CGroundManager.h"

#include "Input/CInputManager.h"
#include "Assets/AssetManager.h"
#include "TimeManager/CTimeManager.h"
#include "Scene/SceneData/CSceneData.h"

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
	//フォントの表示パターンを設定
	void SetFontPattern();
	//フォントの座標を設定
	void SetFontPos();

	//選択肢の座標を設定.
	void SetSelectorPos();

	void MoveSelector();

	//選択肢がスティックで連続して動いてしまうのを制御する
	void SelectorControl();
private:
	CDirectX11*			m_pDx11;

	std::vector<std::function<void()>>	m_Action;	//画面遷移時の動作を入れる.

	//リザルト画面に表示するフォントUIの数
	uint8_t	m_ResultFontIndex;
	
	//WINNERかDRAWの文字のどちらかを表示する
	std::unique_ptr<CUIObject>	m_pSpriteResultFont;
	//UI表示フォント
	std::vector<std::unique_ptr<CUIObject>>	m_pSpriteResultUIFont;
	//選択矢印.
	std::unique_ptr<CUIObject>	m_pSpriteSelector;	

	//プレイヤーマネージャークラス.
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;
	
	//地面マネージャークラス.
	std::unique_ptr<CGroundManager> 	m_pGroundManager;

	//フォントの表示パターン
	std::vector<SHORT>	m_FontPatternNo;

	//フォントの座標
	std::vector<D3DXVECTOR3>	m_FontPos;

	//選択矢印の座標.
	std::vector<D3DXVECTOR3>	m_SelectorPos;

	//8ビットの符号付整数型.
	//現在選択中の番号.
	uint8_t						m_SelectorNumber;

	//スティックの上下を倒した時の保存.
	bool isHeldUp;
	bool isHeldDown;

	//フレームカウント.
	int cnt;

	std::vector<::EsHandle>		m_FireworksEffect;	//花火エフェクト.
	int							m_FireworksMax;		//花火の最大数.
};
