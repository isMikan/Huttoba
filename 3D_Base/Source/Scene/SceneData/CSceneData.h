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

    //準備しているかの切り替え.
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

    //準備しているかの取得.
    static bool GetSlot(int index) {
        return Instance().m_IsSlot[index];
    }
    //全て準備中にする.
    static void AllSlotStandby() {
        for (int index = 0; index < Player_Max; index++)
        {
            Instance().m_IsSlot[index] = false;
        }
    }

    //プレイヤーが生きているのか設定.
    static void SetPlayerLive(int index, bool live) {
        Instance().m_IsPlayerLiving[index] = live; 
    }

    //生きているプレイヤーの数.
    static int GetPlayerLivingNum() {
        int count = 0;
        for (int index = 0; index < Player_Max; index++)
        {
            //生きている場合.
            if (Instance().m_IsPlayerLiving[index]) count++;
        }
        return count;
    }

    //プレイヤーが生きているかを初期化.
    static void PlayerAllLive() {
        for (int index = 0; index < Player_Max; index++)
        {
            Instance().m_IsPlayerLiving[index] = true;
        }
    }

    //プレイヤーが生きているのか.
    static bool GetPlayerLiving(int index)
    {
        return Instance().m_IsPlayerLiving[index];
    }

private:
    std::array<bool, Player_Max>    m_IsSlot;           //仮置き
    std::array<bool, Player_Max>    m_IsPlayerLiving;   //プレイヤーが生きているか.
};