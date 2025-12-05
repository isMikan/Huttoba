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
	void HandleItemAction();

	//回避行動
	void AvoidDanger();

private:
	//動きの優先度のスコア
	float	m_MoveScore;

	//アイテム距離スコアの調整係数
	float	m_DistanceWeight;
};