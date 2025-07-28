#pragma once
class CCreateWindow
{
public:
	static CCreateWindow* GetInstance();


	//ウィンドウ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y,
		INT width, INT height);

	//ウィンドウ関数（メッセージ毎の処理）.
	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	HWND GetHundle(){return m_hWnd;}

private:
	CCreateWindow();
	~CCreateWindow();


	// 生成やコピーを削除.
	CCreateWindow(const CCreateWindow& rhs) = delete;
	CCreateWindow& operator = (const CCreateWindow& rhs) = delete;

private:
	HWND			m_hWnd;	//ウィンドウハンドル.

};