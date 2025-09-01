#pragma once

#include "ItemFactory/ItemFactory.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

class ItemBase :
	public CStaticMeshObject
{
public:
	ItemBase();
	~ItemBase();

	/*****************************************************************************************
	* @brief    アイテムを登録するためのテンプレート関数
	* @param    登録したいアイテムのID
	* @return   bit数が少ないboolを利用しているだけで、引数に意味はない
	*****************************************************************************************/

	template<typename T>
	static bool AutoRegister(const std::string& itemId)
	{
		ItemFactory::GetInstance()->RegisterItem(itemId, []()
			{
				return std::make_unique<T>();
			});
		return true;
	}

	/*****************************************************************************************
	* @brief    初期化
	******************************************************************************************/
	virtual void Init() = 0; // 

	/*****************************************************************************************
	* @brief    更新処理
	*****************************************************************************************/
	virtual void Update() override;

	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

protected:

	std::unique_ptr<CStaticMeshObject> m_Item; // アイテム;
};