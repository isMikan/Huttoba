#pragma once
#include "CSound.h"		//サウンドクラス.

/**************************************************
*	サウンドマネージャークラス.
*	Manager(マネージャー)：管理者.
*	.cppのコメントにある1,2,3の順番で追加できる
**/
class CSoundManager
{
public:
	//サウンドリスト列挙型.
	enum enSoundList
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
	bool LoadData(HWND hWnd);

	//SEを再生する.
	void PlaySE(enSoundList list) {
		m_pSound[list]->PlaySE();
	}
	//ループ再生する.
	void PlayLoop(enSoundList list) {
		m_pSound[list]->PlayLoop();
	}
	//停止する.
	void Stop(enSoundList list) {
		m_pSound[list]->Stop();
	}

private:
	std::unordered_map<enSoundList,std::shared_ptr<CSound>> m_pSound;
};