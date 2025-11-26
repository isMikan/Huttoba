#pragma once

#include "Item/ItemBase.h"

class Haetataki
	: public ItemBase
{
public:

	Haetataki();
	~Haetataki();

	/*****************************************************************************************
	* @brief    初期化
	******************************************************************************************/
	void Init() override;


	/*****************************************************************************************
	* @brief    更新処理
	*****************************************************************************************/
	void Update() override;


	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;


	/*****************************************************************************************
	* @brief    アイテムの共通処理
	*****************************************************************************************/
	void Spawn()	override; // 出現
	void OnGround() override; // 取得待機
	void Have()		override; // 取得
	void Use()		override; // 使用
	void Throw()	override; // 投擲
	void Destroy()	override; // 消滅
	void ItemState(IItemObserver::State state)override; //状態変化時に一度だけ処理

	/*****************************************************************************************
	* @brief    ハエたたき取得モーション
	*****************************************************************************************/
	void TakeMostion();

	/*****************************************************************************************
	* @brief    ハエたたき攻撃モーション
	* @return   true:モーション中  false:モーション終了
	*****************************************************************************************/
	bool AttackMostion();

	void OnCollision(CollisionBase* other)override;

	//吹き飛ばし処理
	void Smash(CPlayer& playiers);

	void ChangeCollider();

private:

	D3DXVECTOR3 m_Offset = { 0.5f, 1.f, 0.f };	//プレイヤーの位置に合わせる(突貫)
	D3DXVECTOR3 m_AddPos = { 0.f,0.f, 0.f };	//モーション移動用変数
	D3DXVECTOR3 m_AddRot = { 0.2f,0.2f,0.f };	//モーション角度調整用変数

	bool m_SwitchDir;		//モーション方向切り替え用
	bool m_IsFlyAway;		//吹き飛び力発生フラグ
	float m_IsFlyAwayPower;	//吹き飛び力

	bool m_IsMissAttack; //攻撃は外れているか(SEのタイミング用)

	std::shared_ptr<CollisionBase>	m_pNowCollider;	//拾う用当たり判定

};