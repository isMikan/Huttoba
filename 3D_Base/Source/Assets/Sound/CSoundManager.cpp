#include "CSoundManager.h"
#include "MyMacro.h"

CSoundManager::CSoundManager()
	: m_pSound()
{
}

CSoundManager::~CSoundManager()
{
}

HRESULT CSoundManager::Create()
{
	// 前提, enSoundListに列挙を追加
    // 1, サウンドのインスタンスを作成.
    m_pSound[enSoundList::BGM_Bonus] = std::make_shared<CSound>();
    m_pSound[enSoundList::SE_Jump] = std::make_shared<CSound>();
    m_pSound[enSoundList::SE_Clear] = std::make_shared<CSound>();
    return S_OK;
}

//サウンドデータ読込関数.
bool CSoundManager::Load(HWND hWnd)
{
    struct SoundInfo
    {
        std::wstring Path;
        std::wstring Name;
    };

    std::unordered_map<enSoundList, SoundInfo> SList;


	// 2, サウンドのパスと名前を設定.
	SList[enSoundList::BGM_Bonus]   = { _T("Data\\Sound\\BGM\\BonusGameHouse.mp3"), _T("BGM_Bonus") };
	SList[enSoundList::SE_Jump]     = { _T("Data\\Sound\\SE\\Jump.wav"),            _T("SE_Jump") };
	SList[enSoundList::SE_Clear]    = { _T("Data\\Sound\\SE\\Clear.wav"),           _T("SE_Clear") };


	// 3, サウンドを開く.
    m_pSound[enSoundList::BGM_Bonus]->Open(SList[enSoundList::BGM_Bonus].Path,  SList[enSoundList::BGM_Bonus].Name, hWnd);
    m_pSound[enSoundList::SE_Jump]  ->Open(SList[enSoundList::SE_Jump].Path,    SList[enSoundList::SE_Jump].Name, hWnd);
    m_pSound[enSoundList::SE_Clear] ->Open(SList[enSoundList::SE_Clear].Path,   SList[enSoundList::SE_Clear].Name, hWnd);

    return true;
}