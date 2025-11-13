#pragma once

class CSceneData
{
public:
    CSceneData();
    ~CSceneData();

    static CSceneData& Instance()
    {
        static CSceneData instance;
        return instance;
    }

    //com作る時にもっと練って作り直しておく
    static void ChangeSlot(int index)
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

    static bool GetSlot(int index)
    {
        return Instance().m_IsSlot[index];
    }

    static void SetPlayerLive(int index, bool live) {
        Instance().m_IsPlayerLiving[index] = live; 
    }

    static void PlayerAllLive() {
        for (int index = 0; index < Player_Max; index++)
        {
            Instance().m_IsPlayerLiving[index] = true;
        }
    }

    static bool GameMainEnd() {
        int count = 0;
        for (int index = 0; index < Player_Max; index++)
        {
            if (Instance().m_IsPlayerLiving[index])
            {
                count++;
            }
        }
        if (count == 1)
        {
            return true;
        }
        return false;
    }

    static bool GetPlayerLiving(int index)
    {
        return Instance().m_IsPlayerLiving[index];
    }

private:
    std::array<bool, Player_Max>    m_IsSlot;           //仮置き
    std::array<bool, Player_Max>    m_IsPlayerLiving;   //プレイヤーが生きているか.
};