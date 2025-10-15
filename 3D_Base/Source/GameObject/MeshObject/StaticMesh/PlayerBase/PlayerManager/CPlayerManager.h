#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CCharacter.h"

#include "GameObject//MeshObject/StaticMesh/CCharacter/PlayerObserver/IPlayerObserver.h"

class CPlayerManager
{
public:
	CPlayerManager(int index);
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
		m_pObserver.push_back(observer);
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
	//キャラクターの色を設定する関数.
	CCharacter::ObjectColor SetCharacterColor(int index);

	//初期位置を設定する関数.
	D3DXVECTOR3 SetDefaultPosition(int index);

	//
	void Notify(IPlayerObserver::PlayerEvent event)
	{
		for (auto& observer : m_pObserver)
		{
			observer->OnNotify(event);
		}
	}

private:
	std::vector<std::unique_ptr<IPlayerObserver>>	m_pObserver;	//プレイヤー0のオブサーバ.
	std::vector<std::unique_ptr<CCharacter>>		m_pPlayers;		//プレイヤー.

	int		m_PlayerID;		//プレイヤー番号.
};