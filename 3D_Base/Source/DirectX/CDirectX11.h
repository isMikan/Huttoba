#pragma once

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

//ヘッダ読込.
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>


//ライブラリ読み込み.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要.


/**************************************************
*	DirectX11 セットアップ.
**/
class CDirectX11
{
public:

	

	/*****************************************************************************************
	* @brief    インスタンスを獲得する
	* @return   このクラスのインスタンス
	*****************************************************************************************/
	static CDirectX11* GetInstance();

	HRESULT Create(HWND hWnd);

	//深度（Ｚ）テストON/OFF切り替え.
	void SetDepth( bool flag );
	//アルファブレンドON/OFF切り替え.
	void SetAlphaBlend( bool flag );

	//バックバッファクリア関数.
	void ClearBackBuffer();
	//表示.
	void Present();

	//デバイスを取得.
	ID3D11Device* GetDevice() const { return m_pDevice11.Get(); }
	//デバイスコンテキストを取得.
	ID3D11DeviceContext* GetContext() const { return m_pContext11.Get(); }

	// ワイヤーフレーム描画に切り替える
	void SetRasterizerWireframe() { m_pContext11->RSSetState(m_pRasterStateWireframe.Get()); }
	// ソリッド描画に戻す
	void SetRasterizerSolid() { m_pContext11->RSSetState(m_pRasterStateSolid.Get()); }

private:
	CDirectX11();
	~CDirectX11();

	// 生成やコピーを削除.
	CDirectX11(const CDirectX11& rhs) = delete;
	CDirectX11& operator = (const CDirectX11& rhs) = delete;


private:
	//デバイスとスワップチェイン作成.
	HRESULT CreateDeviceAndSwapChain(
		HWND hWnd, UINT uFPS, UINT uWidth, UINT uHeight );

	//バックバッファ作成:カラー用レンダーターゲットビュー作成.
	HRESULT CreateColorBackBufferRTV();

	//バックバッファ作成:デプスステンシル用レンダーターゲットビュー作成.
	HRESULT CreateDepthStencilBackBufferRTV();

	//ラスタライザ作成.
	HRESULT CreateRasterizer();

	//デプスステンシルステート作成.
	HRESULT CreateDepthStencilState();

	//アルファブレンドステート作成.
	HRESULT CreateAlphaBlendState();

private:
	ComPtr<ID3D11Device>			m_pDevice11;			//デバイスオブジェクト.
	ComPtr<ID3D11DeviceContext>		m_pContext11;			//デバイスコンテキスト.
	ComPtr<IDXGISwapChain>			m_pSwapChain;			//スワップチェーン.
	ComPtr<ID3D11RenderTargetView>	m_pBackBuffer_TexRTV;	//レンダーターゲットビュー.
	ComPtr<ID3D11Texture2D>			m_pBackBuffer_DSTex;	//デプスステンシル用テクスチャ.
	ComPtr<ID3D11DepthStencilView>	m_pBackBuffer_DSTexDSV;	//デプスステンシルビュー.

	//深度（Ｚ）テスト設定.
	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilStateOn;		//有効設定.
	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilStateOff;	//無効設定.

	//アルファブレンド.
	ComPtr<ID3D11BlendState>		m_pAlphaBlendOn;	//有効設定.
	ComPtr<ID3D11BlendState>		m_pAlphaBlendOff;	//無効設定.

	//ラスタライザステート.
	ComPtr<ID3D11RasterizerState>	m_pRasterStateSolid;
	ComPtr<ID3D11RasterizerState>	m_pRasterStateWireframe;
};