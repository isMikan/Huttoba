#pragma once

#include"StaticMesh/CStaticMesh.h"
#include"SkinMesh/CSkinMesh.h"

/**************************************************
*	メッシュマネージャークラス
* 　担当者: 佐藤　隼斗
*
* 　@brief    メッシュの管理を行うクラス
**/

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	/*****************************************************************************************
	* @brief    メッシュのインスタンス作成
	* @return   成功 : S_OK, 
	*			失敗 : E_FAIL
	*****************************************************************************************/
	HRESULT Create();


	/*****************************************************************************************
	* @brief    メッシュのデータを読み込み
	* @return   成功 : S_OK, 
	*			失敗 : E_FAIL
	*****************************************************************************************/
	HRESULT LoadData();

	/*****************************************************************************************
	* @brief    スタティックメッシュの取得
	* @param    欲しいメッシュのID
	* @return   指定したメッシュのポインタ
	*****************************************************************************************/
	std::shared_ptr<CStaticMesh> GetStaticMesh(StaticMeshList meshId);

	/*****************************************************************************************
	* @brief    スキンメッシュの取得
	* @param    欲しいメッシュのID
	* @return   指定したメッシュのポインタ
	*****************************************************************************************/
	std::shared_ptr<CSkinMesh> GetSkinMesh(SkinMeshList meshId);

private:


	MeshManager(MeshManager& shr) = delete;
	MeshManager& operator=(const MeshManager& rhs) = delete;

private:

	HRESULT CreateStaticMesh();
	HRESULT CreateSkinMesh();
	HRESULT LoadStaticMesh();
	HRESULT LoadSKinMesh();

private:

	//スタティックメッシュ(使いまわす資源)
	std::unordered_map<StaticMeshList, std::shared_ptr<CStaticMesh>> m_pStaticMeshMap;

	//スキンメッシュ(使いまわす資源)
	std::unordered_map<SkinMeshList, std::shared_ptr<CSkinMesh>> m_pSkinMeshMap;

};