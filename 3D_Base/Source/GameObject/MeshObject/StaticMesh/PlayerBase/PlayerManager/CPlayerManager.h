#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/***********************************************************************
*   プレイヤーマネージャークラス.
**/
class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	//--- 構築関数 ---.
	void Create();
	//--- 読込関数 ---.
	void LoadData();
	//--- 破棄関数 ---.
	void Destroy();
	//--- 更新関数 ---.
	void Update();
	//--- 描画関数 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

	//今は実際使うかわからないのでコメント化.
	//エフェクトを表示するための関数.
	//void ManageEffectLaser(static::EsHandle hEffect);

	//一人のプレイヤーを取得.
	CPlayerBase* GetPlayer(int id) { return m_pPlayers[id].get(); }
	std::vector<std::unique_ptr<CPlayerBase>>& GetPlayer() { return m_pPlayers; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- キャラクターの色を設定する関数 ---.
	CPlayerBase::ObjectColor SetCharacterColor(int index);

	//--- 初期位置を設定する関数 ---.
	D3DXVECTOR3 SetDefaultPosition(int index);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CPlayerBase>>		m_pPlayers;		//プレイヤー.

	int		m_PlayerID;		//プレイヤー番号.
};