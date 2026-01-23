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
    m_SoundInfoList.push_back({ enSoundList::BGM_SceneTitle,    _T("Data\\Sound\\BGM\\SceneTitle.mp3"),     _T("BGM_SceneTitle") });
    m_SoundInfoList.push_back({ enSoundList::BGM_SceneStanby,   _T("Data\\Sound\\BGM\\SceneStanby.mp3"),    _T("BGM_SceneStanby") });
    m_SoundInfoList.push_back({ enSoundList::BGM_SceneMain,     _T("Data\\Sound\\BGM\\SceneMain.mp3"),      _T("BGM_SceneMain") });
    m_SoundInfoList.push_back({ enSoundList::BGM_SceneResult,   _T("Data\\Sound\\BGM\\SceneResult.mp3"),    _T("BGM_SceneResult") });
    m_SoundInfoList.push_back({ enSoundList::SE_Jump,           _T("Data\\Sound\\SE\\Jump.wav"),            _T("SE_Jump") });
    m_SoundInfoList.push_back({ enSoundList::SE_Clear,          _T("Data\\Sound\\SE\\Clear.wav"),           _T("SE_Clear") });
    m_SoundInfoList.push_back({ enSoundList::SE_Pickup,         _T("Data\\Sound\\SE\\Pickup.mp3"),          _T("SE_Pickup") });
    m_SoundInfoList.push_back({ enSoundList::SE_AttackHand,     _T("Data\\Sound\\SE\\AttackHand.mp3"),      _T("SE_AttackHand") });
    m_SoundInfoList.push_back({ enSoundList::SE_Whiff,          _T("Data\\Sound\\SE\\Whiff.mp3"),           _T("SE_Whiff") });
    m_SoundInfoList.push_back({ enSoundList::SE_Down,           _T("Data\\Sound\\SE\\Down.mp3"),            _T("SE_Down") });
    m_SoundInfoList.push_back({ enSoundList::SE_Knockback,      _T("Data\\Sound\\SE\\Knockback.mp3"),       _T("SE_Knockback") });
    m_SoundInfoList.push_back({ enSoundList::SE_Knockdown,      _T("Data\\Sound\\SE\\Knockdown.mp3"),       _T("SE_Knockdown") });
    m_SoundInfoList.push_back({ enSoundList::SE_HitHand,        _T("Data\\Sound\\SE\\HitHand.mp3"),         _T("SE_HitHand") });
    m_SoundInfoList.push_back({ enSoundList::SE_Warning,        _T("Data\\Sound\\SE\\Warning.mp3"),         _T("SE_Warning") });
    m_SoundInfoList.push_back({ enSoundList::SE_HitHaetataki,       _T("Data\\Sound\\SE\\HitHaetataki.mp3"),        _T("SE_HitHaetataki") });
    m_SoundInfoList.push_back({ enSoundList::SE_MissHaetataki,      _T("Data\\Sound\\SE\\MissHaetataki.mp3"),       _T("SE_MissHaetataki") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangMaxCharge, _T("Data\\Sound\\SE\\BoomerangMaxCharge.mp3"),  _T("SE_BoomerangMaxCharge") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangCharge,    _T("Data\\Sound\\SE\\BoomerangCharge.mp3"),     _T("SE_BoomerangCharge") });
    m_SoundInfoList.push_back({ enSoundList::SE_BoomerangThrow,     _T("Data\\Sound\\SE\\BoomerangThrow.mp3"),      _T("SE_BoomerangThrow") });
    m_SoundInfoList.push_back({ enSoundList::SE_DestroyItem,        _T("Data\\Sound\\SE\\DestroyItem.mp3"),         _T("SE_DestroyItem") });
    m_SoundInfoList.push_back({ enSoundList::SE_SmashBatHit,        _T("Data\\Sound\\SE\\SmashBatHit.mp3"),         _T("SE_SmashBatHit") });
    m_SoundInfoList.push_back({ enSoundList::SE_FunUse,             _T("Data\\Sound\\SE\\FunUse.mp3"),              _T("SE_FunUse") });
    m_SoundInfoList.push_back({ enSoundList::SE_HandClap,           _T("Data\\Sound\\SE\\HandClap.mp3"),                _T("SE_HandClap") });
    m_SoundInfoList.push_back({ enSoundList::SE_FieWork,            _T("Data\\Sound\\SE\\firework.mp3"),                _T("SE_firework") });
    m_SoundInfoList.push_back({ enSoundList::SE_Piko,               _T("Data\\Sound\\SE\\Piko.mp3"),                _T("SE_Piko") });
    m_SoundInfoList.push_back({ enSoundList::SE_EndSceneMain,       _T("Data\\Sound\\SE\\EndSceneMain.mp3"),        _T("SE_EndSceneMain") });

    m_SoundInfoList.push_back({ enSoundList::SE_Explosion,          _T("Data\\Sound\\SE\\Explosion.mp3"),           _T("SE_Explosion") });

    m_SoundInfoList.push_back({ enSoundList::SE_MoveSelectionArrow, _T("Data\\Sound\\SE\\MoveSelectArrow.mp3"),     _T("SE_MoveSelectArrow") });
    m_SoundInfoList.push_back({ enSoundList::SE_Decision,           _T("Data\\Sound\\SE\\Decision.mp3"),            _T("SE_Decision") });
    m_SoundInfoList.push_back({ enSoundList::SE_PreparationSwitch,  _T("Data\\Sound\\SE\\PreparationSwitch.mp3"),   _T("SE_PreparationSwitch") });
    m_SoundInfoList.push_back({ enSoundList::SE_Ready,              _T("Data\\Sound\\SE\\Ready.mp3"),               _T("SE_Ready") });
    m_SoundInfoList.push_back({ enSoundList::SE_Go,                 _T("Data\\Sound\\SE\\Go.mp3"),                  _T("SE_Go") });
    m_SoundInfoList.push_back({ enSoundList::SE_Finish,             _T("Data\\Sound\\SE\\Explosion.mp3"),           _T("SE_Finish") });


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
