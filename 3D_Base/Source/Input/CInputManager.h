#pragma once
#include "CInput.h"
#include <array>

//準備画面でのプレイヤーの状態(※絶対ここに書くべきではないので学校でどこに入れるかみんなに聞く).
enum class PlayerSlotState
{
	None,
	Player,
	COM,

	Max
};

//プレイヤーの情報をまとめる(※上に同じ).
struct PlayerSlot
{
	PlayerSlotState state;	//準備画面でのプレイヤーの状態.
    bool ready = false;			    //準備状態.
};

class CInputManager
{
public:
    ~CInputManager();

    static CInputManager& Instance() 
    {
        static CInputManager instance;
        return instance;
    }

    //Inputのupdate回している
    static void Update();

    //第2引数で指定のInputに入力設定
    static void BindKey(Action action, const InputBinding& binding, int index);

    //第2引数で指定したInputのIsDown
    static bool IsDown(Action action, int index);
    
    //第2引数で指定したInputのIsUp
    static bool IsUp(Action action, int index);

    //第2引数で指定したInputのIsRepeat
    static bool IsRepeat(Action action, int index);
    
    //引数で指定したパッドの左スティックの正規化したX軸を取得
    static float GetLeftSthikX(int index);
    //引数で指定したパッドの左スティックの正規化したY軸を取得
    static float GetLeftSthikY(int index);
    //引数で指定したパッドの右スティックの正規化したX軸を取得
    static float GetRightSthikX(int index);
    //引数で指定したパッドの右スティックの正規化したY軸を取得
    static float GetRightSthikY(int index);
    
    //引数で指定したパッドが接続されているかを取得
    static bool IsConnect(int index) { return Instance().m_pInputs[index]->IsConnect(); }

    //別の場所に追加するべき----------
    static PlayerSlot& GetSlot(int index) { return Instance().m_Slot[index]; }

    //com作る時にもっと練って作り直しておく
    static void ChangeSlot(int index)
    {
        if (Instance().m_Slot[index].ready)
        {
            Instance().m_Slot[index].ready = false;
        }
        else
        {
            Instance().m_Slot[index].ready = true;
        }
    }
    //CInputのものではない
    static void SetHuman(int index)
    {
        Instance().m_Slot[index].state = PlayerSlotState::Player;
        Instance().m_Slot[index].ready = true;
    }
    //CInputのものではない
    static void SetCPU(int index)
    {
        Instance().m_Slot[index].state = PlayerSlotState::COM;
        Instance().m_Slot[index].ready = false;
    }

    /// <summary>
    /// プレイヤースロットの準備状態を取得
    /// </summary>
    /// <param name="index">取得したいプレイヤー番号</param>
    /// <returns>引数の番号のプレイヤーがtrueなら人,falseならCOM</returns>
    bool GetSlotRedy(int index)
    {
        return Instance().m_Slot[index].ready;
    }
    //-------

private:
    CInputManager();
    CInputManager(const CInputManager&) = delete;
    CInputManager& operator=(const CInputManager&) = delete;

private:
    std::array<std::unique_ptr<CInput>, 4>  m_pInputs;

    std::array<PlayerSlot, 4>                m_Slot;    //仮置き
};