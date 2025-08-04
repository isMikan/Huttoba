#pragma once

#include"Sprite2D/CSprite2D.h"
#include"Sprite3D/CSprite3D.h"

class SpriteManager
{
public:

	//外部から情報を取得するための前設計。まだ使いにくいかもだけど許してください。
	struct Sprite2DInfo
	{
		Sprite2DList Id;
		LPCTSTR Path;
		CSprite2D::SPRITE_STATE Size;
	};

	struct Sprite3DInfo
	{
		Sprite3DList Id;
		LPCTSTR Path;
		CSprite3D::SPRITE_STATE Size;
	};


	SpriteManager();
	~SpriteManager();

	/*****************************************************************************************
	* @brief    各スプライトのCerate関数を回す関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT Create();


	/*****************************************************************************************
	* @brief    各スプライトのLoadData関数を回す関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT LoadData();


	/*****************************************************************************************
	* @brief    各スプライト3Dの情報を取得する関数
	* @param    欲しいスプライトの列挙子ID
	* @return   指定したスプライトのポインタ
	*****************************************************************************************/
	std::shared_ptr<CSprite3D> GetSprite3D(Sprite3DList spriteID);


	/*****************************************************************************************
	* @brief    各スプライト2Dの情報を取得する関数
	* @param    欲しいスプライトの列挙子ID
	* @return   指定したスプライトのポインタ
	*****************************************************************************************/
	std::shared_ptr<CSprite2D> GetSprite2D(Sprite2DList spriteID);

private:

	/*****************************************************************************************
	* @brief    Sprite3Dの情報登録とインスタンス作成を行う関数
	* @return   S_OK : 成功,
	*			E_POiNTER : ポインタがnullptr,
	*****************************************************************************************/
	HRESULT CreateSprite3D();


	/*****************************************************************************************
	* @brief    Sprite2Dの情報登録とインスタンス作成を行う関数
	* @return   S_OK : 成功,
	*			E_POiNTER : ポインタがnullptr,
	*****************************************************************************************/
	HRESULT CreateSprite2D();


	/*****************************************************************************************
	* @brief    登録した情報をもとに、sprite2DMapの紐づけを行う関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT LoadSprite2D();


	/*****************************************************************************************
	* @brief    登録した情報をもとに、sprite3DMapの紐づけを行う関数
	* @return   S_OK : 成功,
	*			E_FAIL : 失敗,
	*****************************************************************************************/
	HRESULT LoadSprite3D();


	/*****************************************************************************************
	* @brief    sprite2Dの登録を行う関数
	* @param    spriteID : 列挙子ID,
	*			path     : ファイル名,
	*			size     : サイズ,
	*****************************************************************************************/
	void RegisterSprite2D(Sprite2DList spriteID, LPCTSTR path, CSprite2D::SPRITE_STATE size);


	/*****************************************************************************************
	* @brief    sprite3Dの登録を行う関数
	* @param    spriteID : 列挙子ID,
	*			path     : ファイル名,
	*			size     : サイズ,
	*****************************************************************************************/
	void RegisterSprite3D(Sprite3DList spriteID, LPCTSTR path, CSprite3D::SPRITE_STATE size);
	

private:
	//ゲームで扱うスプライトデータ
	std::unordered_map<Sprite3DList, std::shared_ptr<CSprite3D> > m_p3DSpritMap;

	//スプライト2Dデータ
	std::unordered_map<Sprite2DList, std::shared_ptr<CSprite2D> > m_p2DSpritMap;

	//スプライト2Dの情報を登録するためのリスト
	std::vector<Sprite2DInfo> m_Sprite2DInfoList;

	//スプライト3Dの情報を登録するためのリスト
	std::vector<Sprite3DInfo> m_Sprite3DInfoList;
};