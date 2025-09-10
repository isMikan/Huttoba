#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

class CPlayerManager
{
public:
	CPlayerManager(int index);
	~CPlayerManager();

	//構築関数.
	HRESULT Create();
	//データの読み込み関数.
	HRESULT LoadData();
	//初期化関数.
	void Init();
	//破棄関数.
	void Destroy();
	//更新関数.
	void Update();
	//描画関数.
	void Draw();

private:
	//キャラクターの色を設定する関数.
	CPlayer::ObjectColor SetCharacterColor(int index);

	//初期位置.
	D3DXVECTOR3 SetDefaultPosition(int index);


private:
	//キャラクタークラス
	std::vector<std::unique_ptr<CPlayer>>	m_pPlayer;

	int m_PlayerID;
};