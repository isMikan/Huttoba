#pragma once

#include "Ground/GroundManager/CGroundManager.h"

struct VECTOR2
{
	float x = 0.f;
	float z = 0.f;
};

class SpawnItemPosition
{
public:

	SpawnItemPosition(std::unique_ptr<CGroundManager>& GManager);
	~SpawnItemPosition();

	//出現位置を設定
	D3DXVECTOR3 SerectPosition();
	D3DXVECTOR3 StartPosition();


	void Init();
	void Uptate();

	//ステージの落ち情報を渡す
	GroundTag GetCurrentFallGround() { return m_CurrentFallGround; }

private:

	//現在のステージの落ち具合をチェック
	void CheckCurrentGround();

	//ランダムで出現位置を設定
	VECTOR2 SetRandomPos();


private:
	//現在のステージ情報を保存
	GroundTag	m_CurrentFallGround;

	//地面クラスの参照
	std::unique_ptr<CGroundManager>&	m_pGroundManager;

	//アイテムの出現幅
	VECTOR2 m_CurrentClampRangeMax;
	VECTOR2 m_CurrentClampRangeMin;

	bool IsFirstSpawn;
	int  FirstSpawnCount;

	std::vector<D3DXVECTOR3> m_pFirstSpawnPosition;
};