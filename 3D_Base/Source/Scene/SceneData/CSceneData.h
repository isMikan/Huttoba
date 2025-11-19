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
    static bool GetSlot(int index)
    {
        return Instance().m_IsSlot[index];
    }

    //プレイヤーが生きているのか設定.
    static void SetPlayerLive(int index, bool live) {
        Instance().m_IsPlayerLiving[index] = live; 
    }

    static void PlayerAllLive() {
        for (int index = 0; index < Player_Max; index++)
        {
            Instance().m_IsPlayerLiving[index] = true;
        }
    }

    //ゲームメインを終了するか.
    static bool GameMainEnd() {
        int count = 0;
        for (int index = 0; index < Player_Max; index++)
        {
            //生きている場合
            if (Instance().m_IsPlayerLiving[index]) count++;
        }
        //生きているのが一人以下の場合.
        if (count <= 1)
        {
            return true;
        }
        return false;
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