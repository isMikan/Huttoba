#pragma once
#include "Item/ItemBase.h"

class SmashBat
	: public ItemBase
{
public:
	SmashBat();
	~SmashBat();

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
	void Have()	override; // 取得
	void Use()	override; // 使用
	void Throw()	override; // 投擲
	void Destroy()	override; // 消滅

	void ItemState(IItemObserver::State state)override; //状態変化時に一度だけ処理

private:
	void OnCollision(CollisionBase* other)override;

private:
};
