#include "CSoundManager.h"
#include "MyMacro.h"

CSoundManager::CSoundManager()
	: m_pSound()
{
}

CSoundManager::~CSoundManager()
{
}


//-----------------------------------------------------------------------

HRESULT CSoundManager::Create()
{
	//サウンドの列挙に対応するパスと名前を定義
    m_SoundInfoList = 
    {
        { enSoundList::BGM_Bonus, _T("Data\\Sound\\BGM\\BonusGameHouse.mp3"), _T("BGM_Bonus") },
        { enSoundList::SE_Jump,   _T("Data\\Sound\\SE\\Jump.wav"),            _T("SE_Jump") },
        { enSoundList::SE_Clear,  _T("Data\\Sound\\SE\\Clear.wav"),           _T("SE_Clear") },
    };

	//サウンドのインスタンスを格納するための配列を確保
    for (auto& sound : m_SoundInfoList)
    {
        // 1, サウンドのインスタンスを作成.
        m_pSound[sound.Id] = std::make_shared<CSound>();
		if (!m_pSound[sound.Id]) return E_FAIL;
    }

    return S_OK;
}

//-----------------------------------------------------------------------

HRESULT CSoundManager::LoadData(HWND hWnd)
{
    for (auto& sound : m_SoundInfoList)
    {
        if(!m_pSound[sound.Id]->Open(sound.Path,sound.Name, hWnd)) return E_FAIL;
    }
    return S_OK;
}

//-----------------------------------------------------------------------
