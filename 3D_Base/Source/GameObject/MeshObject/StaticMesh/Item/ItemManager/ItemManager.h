#pragma once
#include "CStaticMeshObject.h"
#include "Ground/GroundManager/CGroundManager.h"

static constexpr int SPAWN_LIMIT = 8;	//ステージ上のアイテム最大出現数


class ItemBase;
class CPlayerManager;
class SelectSpawnItem;
class SpawnItemPosition;

class ItemManager:
	public CStaticMeshObject
{
public:
	ItemManager(std::unique_ptr<CGroundManager>& GManager);
	~ItemManager();

	/*****************************************************************************************
	* @brief    生成
	*****************************************************************************************/
	HRESULT Create();


	/*****************************************************************************************
	* @brief    データを読み込む
	*****************************************************************************************/
	HRESULT LoadData();


	/*****************************************************************************************
	* @brief    初期化
	*****************************************************************************************/
	void Init();


	/*****************************************************************************************
	* @brief    更新処理
	*****************************************************************************************/
	void Update() ;

	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	/*****************************************************************************************
	* @brief    アイテムの作成
	*****************************************************************************************/
	void CreateItem();

	/*****************************************************************************************
	* @brief    破壊フラグを参照して破壊を行う関数
	*****************************************************************************************/
	void DestroyItem(); 

	//現在出現中のアイテムの数を取得
	size_t GetItemVectorNum() { return m_pItems.size(); }

	std::vector<std::unique_ptr<ItemBase>>& GetItems() { return m_pItems; }

	

	//出現中のアイテムの位置を取得
	D3DXVECTOR3 GetItemPos(int i);

	//void SetSpawnItemPosition(SpawnItemPosition& posClass) { m_pSpawnPosition = posClass; }

private:

	// アイテムのコンテナ
	std::vector<std::unique_ptr<ItemBase>>  m_pItems;
	std::unique_ptr<SelectSpawnItem>	    m_pSpawnItem;
	std::unique_ptr<SpawnItemPosition>		m_pSpawnItemPosition;
	

};