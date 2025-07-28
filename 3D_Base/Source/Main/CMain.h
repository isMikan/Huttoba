#pragma once
#include <Windows.h>

//クラスの前方宣言.
class CDirectX9;
class CDirectX11;
class CGame;

/**************************************************
*	メインクラス.
**/
class CMain
{
public:
	CMain();	//コンストラクタ.
	~CMain();	//デストラクタ.

	void Update();		//更新処理.
	HRESULT Create();	//構築処理.
	HRESULT LoadData();	//データロード処理.
	void Release();		//解放処理.
	void Loop();		//メインループ.

private:

private:
	CGame*			m_pGame;
};