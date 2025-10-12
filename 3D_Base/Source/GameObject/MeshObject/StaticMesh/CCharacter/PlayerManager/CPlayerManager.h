#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CCharacter.h"

#include "CInput/CInputManager.h"

#include <unordered_map>

class CPlayerManager
{
public:
	CPlayerManager(int index);
	~CPlayerManager();

	//構築関数.
	HRESULT Create();
	//データの読み込み関数.
	HRESULT LoadData();
	//破棄関数.
	void Destroy();
	//更新関数.
	void Update();
	//描画関数.
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

	//衝突判定関数.
	void Collision();
	
	//入力関数.
	void HandleInput();

	//今は実際使うかわからないのでコメント化.
	//エフェクトを表示するための関数.
	//void ManageEffectLaser(static::EsHandle hEffect);

private:
	//キャラクターの色を設定する関数.
	CPlayer::ObjectColor SetCharacterColor(int index);

	//初期位置を設定する関数.
	D3DXVECTOR3 SetDefaultPosition(int index);

	//キーバインドを設定する関数.
	void SetPlayerInputBinding();

private:
	std::vector<std::unique_ptr<CCharacter>> m_pPlayers;	//プレイヤー.

	int		m_PlayerID;		//プレイヤー番号.
};