#pragma once
#include "Scene/CScene.h"

#include "Sprite/Sprite2D/CSprite2D.h"
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
private:
	std::unique_ptr<CSprite2D>	m_pSpriteStandbyImg;	//待機画面.

	std::unique_ptr<CCamera>	m_pCamera;	//カメラ.

	//キャラクタークラス
	std::vector< std::unique_ptr<CPlayer> >	m_pPlayer;

	CInput&		m_Input;
};
