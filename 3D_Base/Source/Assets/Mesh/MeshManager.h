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

	//外部から情報を取得するための前設計。まだ使いにくいかもだけど許してください。
	struct StaticMeshInfo
	{
		StaticMeshList Id;
		LPCTSTR Path;
	};

	struct SkinMeshInfo
	{
		SkinMeshList Id;
		LPCTSTR Path;
	};

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

	/*****************************************************************************************
	* @brief    StaticMeshの情報登録とインスタンス作成を行う関数
	* @return   S_OK : 成功,
	*			E_POiNTER : ポインタがnullptr,
	*****************************************************************************************/
	HRESULT CreateStaticMesh();


	/*****************************************************************************************
	* @brief    SkinMeshの情報登録とインスタンス作成を行う関数
	* @return   S_OK : 成功,
	*			E_POiNTER : ポインタがnullptr,
	*****************************************************************************************/
	HRESULT CreateSkinMesh();


	/*****************************************************************************************
	* @brief    登録した情報をもとに、StaticMeshMapに紐づけを行う関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT LoadStaticMesh();


	/*****************************************************************************************
	* @brief    登録した情報をもとに、SkinMeshMapに紐づけを行う関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT LoadSKinMesh();


	/*****************************************************************************************
	* @brief    スタティックメッシュの登録を行う関数
	* @param    spriteID : 列挙子ID,
	*			path     : ファイル名,
	*****************************************************************************************/
	void RegisterStaticMesh(StaticMeshList spriteID, LPCTSTR path);


	/*****************************************************************************************
	* @brief    スキンメッシュの登録を行う関数
	* @param    spriteID : 列挙子ID,
	*			path     : ファイル名,
	*****************************************************************************************/
	void RegisterSkinMesh(SkinMeshList spriteID, LPCTSTR path);

private:

	//スタティックメッシュ(使いまわす資源)
	std::unordered_map<StaticMeshList, std::shared_ptr<CStaticMesh>> m_pStaticMeshMap;

	//スキンメッシュ(使いまわす資源)
	std::unordered_map<SkinMeshList, std::shared_ptr<CSkinMesh>> m_pSkinMeshMap;

	//スプライト2Dの情報を登録するためのリスト
	std::vector<StaticMeshInfo> m_StaticMeshInfoList;

	//スプライト3Dの情報を登録するためのリスト
	std::vector<SkinMeshInfo> m_SkinMeshInfoList;

};