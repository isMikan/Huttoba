#pragma once

#include "PlayerBase/PlayerAI/CPlayerAI.h"

class CPlayerAI_TypeB
	: public CPlayerAI	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI_TypeB(int index);
	~CPlayerAI_TypeB() override;

	//--- 毎フレームの動作 ---.
	void Update() override;
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
private:
	//アイテムの探索
	void SearchItem();

	//アイテムを所持中の動き
	void HaveItem();

private:
	//アイテム距離スコアの調整係数
	float m_DistanceWeight;
};