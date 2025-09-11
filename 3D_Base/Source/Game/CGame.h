#pragma once
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "DebugText/CDebugText.h"
#include "Assets/Sprite/Sprite3D/CSprite3D.h"
#include "Assets/Sprite/Sprite2D/CSprite2D.h"
#include "GameObject/SpriteObject/CSpriteObject.h"
#include "GameObject/SpriteObject/Explosion/CExplosion.h"
#include "GameObject/UIObject/CUIObject.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CCharacter.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"
#include "GameObject/MeshObject/StaticMesh/Ground/CGround.h"
#include "Scene/CSceneManager.h"


#include<string>
#include<vector>
#include<unordered_map>
#include<memory>

/********************************************************************************
*	ゲームクラス.
**/
class CGame
{
public:

	CGame( HWND hWnd );
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();

	void Update();
	void Draw();

private:
	HWND			m_hWnd;			//ウィンドウハンドル

	std::unique_ptr<CSceneManager> m_pSceneManager;

private:
	//=delete「削除定義」と呼ばれる機能.
	//指定された場合、その関数は呼び出せなくなる.
	CGame() = delete;	//デフォルトコンストラクタ禁止.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};