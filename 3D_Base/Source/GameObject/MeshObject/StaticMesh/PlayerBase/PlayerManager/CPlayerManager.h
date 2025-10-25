#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/**************************************************
*   プレイヤーマネージャークラス.
**/
class CPlayerManager
	: public ICollisionListener
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

	//--- 衝突判定の実装 ---.
	void OnCollision(CollisionBase* pOtherCollider) override;

	//今は実際使うかわからないのでコメント化.
	//エフェクトを表示するための関数.
	//void ManageEffectLaser(static::EsHandle hEffect);

	//一人のプレイヤーを取得.
	CPlayerBase* GetPlayer(int id) { return m_pPlayers[id].get(); }

private:
	//--- キャラクターの色を設定する関数 ---.
	CPlayerBase::ObjectColor SetCharacterColor(int index);

	//--- 初期位置を設定する関数 ---.
	D3DXVECTOR3 SetDefaultPosition(int index);

private:
	std::vector<std::unique_ptr<CPlayerBase>>		m_pPlayers;		//プレイヤー.

	int		m_PlayerID;		//プレイヤー番号.
};