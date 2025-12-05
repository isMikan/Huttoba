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
    m_SoundInfoList.push_back({ enSoundList::BGM_Bonus,         _T("Data\\Sound\\BGM\\BonusGameHouse.mp3"), _T("BGM_Bonus") });
    m_SoundInfoList.push_back({ enSoundList::BGM_SceneMain,     _T("Data\\Sound\\BGM\\SceneMain.mp3"),      _T("BGM_SceneMain") });
    m_SoundInfoList.push_back({ enSoundList::SE_Jump,           _T("Data\\Sound\\SE\\Jump.wav"),            _T("SE_Jump") });
    m_SoundInfoList.push_back({ enSoundList::SE_Clear,          _T("Data\\Sound\\SE\\Clear.wav"),           _T("SE_Clear") });
    m_SoundInfoList.push_back({ enSoundList::SE_Pickup,         _T("Data\\Sound\\SE\\Pickup.mp3"),          _T("SE_Pickup") });
    m_SoundInfoList.push_back({ enSoundList::SE_AttackHand,     _T("Data\\Sound\\SE\\AttackHand.mp3"),      _T("SE_AttackHand") });
    m_SoundInfoList.push_back({ enSoundList::SE_Whiff,          _T("Data\\Sound\\SE\\Whiff.mp3"),           _T("SE_Whiff") });
    m_SoundInfoList.push_back({ enSoundList::SE_Down,           _T("Data\\Sound\\SE\\Down.mp3"),            _T("SE_Down") });
    m_SoundInfoList.push_back({ enSoundList::SE_Knockback,      _T("Data\\Sound\\SE\\Knockback.mp3"),       _T("SE_Knockback") });
    m_SoundInfoList.push_back({ enSoundList::SE_Knockdown,      _T("Data\\Sound\\SE\\Knockdown.mp3"),       _T("SE_Knockdown") });
    m_SoundInfoList.push_back({ enSoundList::SE_HitHand,        _T("Data\\Sound\\SE\\HitHand.mp3"),         _T("SE_HitHand") });
    m_SoundInfoList.push_back({ enSoundList::SE_HitHaetataki,   _T("Data\\Sound\\SE\\HitHaetataki.mp3"),    _T("SE_HitHaetataki") });
    m_SoundInfoList.push_back({ enSoundList::SE_MissHaetataki,  _T("Data\\Sound\\SE\\MissHaetataki.mp3"),   _T("SE_MissHaetataki") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangMaxCharge,_T("Data\\Sound\\SE\\BoomerangMaxCharge.mp3"),   _T("SE_BoomerangMaxCharge") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangCharge,   _T("Data\\Sound\\SE\\BoomerangCharge.mp3"),      _T("SE_BoomerangCharge") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangThrow,    _T("Data\\Sound\\SE\\BoomerangThrow.mp3"),      _T("SE_BoomerangThrow") });

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
