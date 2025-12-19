#pragma once

#include "Item/ItemBase.h"

class SmashBat
	: public ItemBase
{
public:

	SmashBat();
	~SmashBat();

	enum class Efect
	{
		Have = 0,
		HitPlayer,
	};

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
	void Smash(CPlayerBase& playiers);

	//投げ
	void OneEnterThrow();

private:


	D3DXQUATERNION m_InitalPlayerQ;
	bool m_IsFirst;

	// ハエたたきの補正角
	D3DXQUATERNION m_Startfix;
	D3DXQUATERNION m_Endfix;


	bool m_SwitchDir;		//モーション方向切り替え用
	bool m_IsFlyAway;		//吹き飛び力発生フラグ
	float m_IsFlyAwayPower;	//吹き飛び力

	bool m_IsMissAttack; //攻撃は外れているか(SEのタイミング用)

	//移動速度
	D3DXVECTOR3 m_Velocity;

	float m_slerpTime = 0.0f; // Slerpの経過時間カウンター

	std::unordered_map<Efect, ::EsHandle> m_hEffect;

	//連続ヒット防止のためにプレイヤーを保存する変数
	std::vector<CPlayerBase*> m_HitPlayer;
};