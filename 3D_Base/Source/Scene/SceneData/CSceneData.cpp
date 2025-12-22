#include "CSceneData.h"

CSceneData::CSceneData()
	: m_IsSlot			()
	, m_IsPlayerLiving	()
{
}

CSceneData::~CSceneData()
{
}

//--- 準備しているかの切り替え ---.
void CSceneData::ChangeSlot(int index)
{
    if (Instance().m_IsSlot[index])
    {
        Instance().m_IsSlot[index] = false;
    }
    else
    {
        Instance().m_IsSlot[index] = true;
    }
}

//--- 全て準備中にする ---.
void CSceneData::AllSlotStandby()
{
    for (int index = 0; index < Player_Max; index++)
    {
        Instance().m_IsSlot[index] = false;
    }
}

//--- プレイヤーが生きているかを初期化 ---.
void CSceneData::PlayerAllLive()
{
    for (int index = 0; index < Player_Max; index++)
    {
        Instance().m_IsPlayerLiving[index] = true;
    }
}
