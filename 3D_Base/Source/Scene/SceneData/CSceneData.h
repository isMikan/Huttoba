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

    //--- 準備しているかの切り替え ---.
    static void ChangeSlot(int index);

    //--- 全て準備中にする ---.
    static void AllSlotStandby();

    //--- プレイヤーが生きているかを初期化 ---.
    static void PlayerAllLive();


    //準備しているかの取得.
    static bool GetSlot(int index) {
        return Instance().m_IsSlot[index];
    }

    //プレイヤーが生きているのか取得と設定.
    static bool GetPlayerLiving(int index) {
        return Instance().m_IsPlayerLiving[index];
    }
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


private:
    std::array<bool, Player_Max>    m_IsSlot;           //準備OKか.
    std::array<bool, Player_Max>    m_IsPlayerLiving;   //プレイヤーが生きているか.
};