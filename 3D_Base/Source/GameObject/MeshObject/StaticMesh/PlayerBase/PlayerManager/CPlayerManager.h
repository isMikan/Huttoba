#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject//MeshObject/StaticMesh/PlayerBase/PlayerObserver/IPlayerObserver.h"

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

	//--- 衝突判定関数 ---.
	void Collision();
	
	void AddObserver(std::unique_ptr<IPlayerObserver> observer) 
	{
		m_pObserver.push_back(std::move(observer));
	}

	void RemoveObserver(std::unique_ptr<IPlayerObserver> observer)
	{
		m_pObserver.erase(
			std::remove(m_pObserver.begin(), m_pObserver.end(), observer),
			m_pObserver.end());
	}

	//今は実際使うかわからないのでコメント化.
	//エフェクトを表示するための関数.
	//void ManageEffectLaser(static::EsHandle hEffect);

private:
	//--- キャラクターの色を設定する関数 ---.
	CPlayerBase::ObjectColor SetCharacterColor(int index);

	//--- 初期位置を設定する関数 ---.
	D3DXVECTOR3 SetDefaultPosition(int index);

	//--- オブサーバに通知する ---.
	void Notify(IPlayerObserver::HitEvent event);

private:
	std::vector<std::unique_ptr<IPlayerObserver>>	m_pObserver;	//プレイヤーのオブサーバ.
	std::vector<std::unique_ptr<CPlayerBase>>		m_pPlayers;		//プレイヤー.

	int		m_PlayerID;		//プレイヤー番号.
};