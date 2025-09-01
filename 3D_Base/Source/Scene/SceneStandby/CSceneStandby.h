#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"

#include "CInput/CInput.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"
#include "Camera/CCamera.h"

#include "Assets/AssetManager.h"

class CSceneStandby
	: public CScene
{
public:
	CSceneStandby(CInput& input);
	~CSceneStandby() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	void InitializePlayers();

	void InitializeInput();
private:
	std::unique_ptr<CUIObject>	m_pSpriteStandbyImg;	//待機画面.

	//プレイヤーの人数の4がマジックナンバーなので後で変更しておく.
	std::array < std::unique_ptr<CUIObject>,4 >	m_pRedyFontImg;		//準備完了画像.
	std::array < std::unique_ptr<CUIObject>,4 >	m_pNotRedyFontImg;	//準備中画像.

	std::unique_ptr<CCamera>	m_pCamera;	//カメラ.

	//キャラクタークラス
	std::array< std::unique_ptr<CPlayer>,4 >	m_pPlayer;

	CInput&				m_Input;
	std::array< std::unique_ptr<CInput>,4>	m_Inputs;
};
