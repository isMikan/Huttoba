#pragma once
#include "CSound.h"		//サウンドクラス.

/**************************************************
*	サウンドマネージャークラス.
*	Manager(マネージャー)：管理者.
*		Singleton(シングルトン：デザインパターンの１つ)で作成.
**/
class CSoundManager
{
public:
	//サウンドリスト列挙型.
	enum enList
	{
		BGM_Bonus,	//ボーナスステージ.
		SE_Jump,	//ジャンプ.
		SE_Clear,	//クリア.

		//音が増えたら「ここ」に追加してください.
		max,		//最大数.
	};

public:
	CSoundManager();
	~CSoundManager();

	HRESULT Create();
	//サウンドデータ読込関数.
	bool Load(HWND hWnd);

	//SEを再生する.
	void PlaySE(enList list) {
		m_pSound[list]->PlaySE();
	}
	//ループ再生する.
	void PlayLoop(enList list) {
		m_pSound[list]->PlayLoop();
	}
	//停止する.
	void Stop(enList list) {
		m_pSound[list]->Stop();
	}

private:
	std::vector<std::shared_ptr<CSound>> m_pSound;
};