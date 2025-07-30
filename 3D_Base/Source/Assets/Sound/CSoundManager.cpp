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
    //インスタンス生成.
    for (int i = 0; i < enList::max; i++)
    {
        m_pSound.push_back(std::make_shared<CSound>());
		if (m_pSound[i] == nullptr)
		{
			OutputDebugString(L"Failed to create CSound instance!\n");
			return E_POINTER;
		}
    }

    return S_OK;
}

//サウンドデータ読込関数.
bool CSoundManager::Load(HWND hWnd)
{
    struct SoundInfo
    {
        enList Id;
        std::wstring Path;
        std::wstring Name;
    };

    std::vector<SoundInfo> SList =
    {
        { enList::SE_Jump,   _T("Data\\Sound\\SE\\Jump.wav"),            _T("SE_Jump") },
        { enList::BGM_Bonus, _T("Data\\Sound\\BGM\\BonusGameHouse.mp3"), _T("BGM_Bonus") },
        { enList::SE_Clear,  _T("Data\\Sound\\SE\\Clear.wav"),           _T("SE_Clear") }
    };

    for (size_t i = 0; i < SList.size(); i++)
    {
        if (!m_pSound[i]) {
            OutputDebugString(L"m_pSound[i] is null!\n");
        }
        else {
            OutputDebugString(L"m_pSound[i] is valid!\n");
        }

        if (m_pSound[i]->Open(SList[i].Path, SList[i].Name, hWnd) == false)
        {
            return false;
        }
    }

    return true;
}