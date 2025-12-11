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
	//アイテムの探索行動
	void SearchItem();

	//アイテムを所持中の行動
	void HandleItemAction();

	//回避行動
	void AvoidDanger();

	//アイテムの種類によるスコアの増減
	float ItemScoreBonus(ItemID item);

	//引数の位置がステージ端にどれくらい近いかでスコアの増減
	float CalculateDangerRate(D3DXVECTOR3 pos);

private:
	//移動先の情報
	NearbyObject	m_Destination;

	//動きの優先度のスコア
	float			m_MoveScore;

	//アイテム距離スコアの調整係数
	float			m_DistanceWeight;
};