#pragma once
#include <functional>

/*************************************************
*
*
*
*/

class ItemBase; // 前方宣言

class ItemFactory
{
public:

	//アイテムのインスタンスを生成する関数の入れ物
	using Createfunc = std::function<std::unique_ptr<ItemBase>()>;


	/*****************************************************************************************
	* @brief    インスタンスを取得する
	* @details  各子クラスで翻訳のタイミングで登録を行っているため、
	*			そのタイミングではインスタンスを生成しておく必要がある。
	*			そのため、シングルトンパターンを採用している
	*
	* @return   このクラスのインスタンスへのポインタ
	*****************************************************************************************/
	static ItemFactory* GetInstance()
	{
		static ItemFactory instance;
		return &instance;
	}

	/*****************************************************************************************
	* @brief    アイテムを登録する
	* @param    itemId		: 登録したいアイテムの名前
	*			Createfunc	: アイテムを生成する関数
	*****************************************************************************************/
	void RegisterItem(const ItemID itemId, const Createfunc& creator);


	/*****************************************************************************************
	* @brief    アイテムを生成する
	* @param    ItemId : 生成したいアイテム名
	* @return   作成したアイテムへのポインタ
	*****************************************************************************************/
	std::unique_ptr<ItemBase> CreateItem(const ItemID ItemId);

private:
	ItemFactory();
	~ItemFactory();

	ItemFactory(const ItemFactory&) = delete; // コピーコンストラクタを削除
	ItemFactory& operator=(const ItemFactory&) = delete; // 代入演算子を削除

private:

	//各アイテムの生成関数を一括保持するためのマップ
	std::unordered_map<ItemID, Createfunc> m_Registry;

};