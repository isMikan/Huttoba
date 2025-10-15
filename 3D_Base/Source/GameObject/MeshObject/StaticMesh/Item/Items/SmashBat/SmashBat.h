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
	void Update(std::vector<std::unique_ptr<CPlayerBase>>& playiers) override;

	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	/*****************************************************************************************
	* @brief    アイテムの共通処理
	*****************************************************************************************/
	void Spawn()	override; // 出現
	void OnGround() override; // 取得待機
	void Have(std::vector<std::unique_ptr<CPlayerBase>>& playiers)	override; // 取得
	void Use(std::vector<std::unique_ptr<CPlayerBase>>& playiers)	override; // 使用
	void Throw()	override; // 投擲
	void Destroy()	override; // 消滅

private:

};
