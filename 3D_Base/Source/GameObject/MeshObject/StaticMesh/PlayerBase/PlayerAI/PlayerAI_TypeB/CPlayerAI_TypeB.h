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

	//アイテムによってそれぞれの移動や攻撃
	float ItemMove(ItemID item);

	//アイテムの種類によるスコアの増減
	float ItemScoreBonus(const ItemID& item)const;

	//ステージ外に近づくと中央に向かう力が強くなる処理
	D3DXVECTOR3 CalculateStage();

	//引数の位置がステージ端にどれくらい近いかでスコアの増減
	float CalculateDangerScore(const D3DXVECTOR3& pos)const;

	//使用された爆弾から逃げる動き
	void RunBomb();

	//回避行動
	D3DXVECTOR3 CalculateAvoidDanger();

private:
	//移動先の情報
	NearbyObject	m_Destination;

	//動きの優先度のスコア
	float			m_MoveScore;

	//アイテム距離スコアの調整係数
	float			m_DistanceWeight;

	//ステージのここより外側は危険とする％
	float			m_GroundSafeRadius;

	//float			m_
};