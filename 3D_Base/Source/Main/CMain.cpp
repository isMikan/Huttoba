#include "CMain.h"
#include "CCreateWindow.h"
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "Game/CGame.h"

//ウィンドウを画面中央で起動を有効にする.
//#define ENABLE_WINDOWS_CENTERING

/////
/********************************************************************************
*	メインクラス.
**/
//=================================================
//	コンストラクタ.
//=================================================
CMain::CMain()
	//初期化リスト.
	: m_pGame	( nullptr )
{
}


//=================================================
//	デストラクタ.
//=================================================
CMain::~CMain()
{
	SAFE_DELETE( m_pGame );
}


//更新処理.
void CMain::Update()
{
	//更新処理.
	m_pGame->Update();

	//バックバッファをクリアにする.
	CDirectX11::GetInstance()->ClearBackBuffer();

	//描画処理.
	m_pGame->Draw();
	
	//画面に表示.
	CDirectX11::GetInstance()->Present();
}


//構築処理.
HRESULT CMain::Create()
{
	//DirectX9構築.
	if (FAILED(CDirectX9::GetInstance()->Create(CCreateWindow::GetInstance()->GetHundle())))
	{
		return E_FAIL;
	}

	//DirectX11構築.
	if( FAILED(CDirectX11::GetInstance()->Create(CCreateWindow::GetInstance()->GetHundle())))
	{
		return E_FAIL;
	}

	//ゲームクラスのインスタンス生成.
	m_pGame = new CGame(CCreateWindow::GetInstance()->GetHundle());

	//ゲームクラスの構築（Loadも含める）.
	m_pGame->Create();

	return S_OK;
}

//データロード処理.
HRESULT CMain::LoadData()
{
	//データロード処理.
	m_pGame->LoadData();

	return S_OK;
}


//解放処理.
void CMain::Release()
{
}


//メッセージループ.
void CMain::Loop()
{
	//データロード.
	if( FAILED( LoadData() )){
		return;
	}
	//------------------------------------------------
	//	フレームレート調整準備.
	//------------------------------------------------
	float Rate = 0.0f;	//レート.
	DWORD sync_old = timeGetTime();			//過去時間.
	DWORD sync_now;							//現在時間.

	//時間処理のため、最小単位を1ミリ秒に変更.
	timeBeginPeriod( 1 );
	Rate = 1000.0f / static_cast<float>(FPS); //理想時間を算出.

	//メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while( msg.message != WM_QUIT )
	{
		sync_now = timeGetTime();	//現在の時間を取得.

		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else if( sync_now - sync_old >= Rate )
		{
			sync_old = sync_now;	//現在時間に置き換え.

			//更新処理.
			Update();
		}
	}
	//アプリケーションの終了.
	Release();
}