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
    void Update();

    //指定したInputをとれる
    CInput& GetInput(int index) { return *m_pInputs[index]; }
    //全部のInputに入力設定
    //残していると問題起こしそうなので消していきます
    void BindKey(Action action, const InputBinding& binding);

    //第2引数で指定のInputに入力設定
    void BindKey(Action action, const InputBinding& binding, int index);

    //第2引数で指定したInputのIsDown
    bool IsDown(Action action, int index);
    
    //第2引数で指定したInputのIsUp
    bool IsUp(Action action, int index);

    //第2引数で指定したInputのIsRepeat
    bool IsRepeat(Action action, int index);
    
    //引数で指定したパッドの左スティックの正規化したX軸を取得
    float GetLeftSthikX(int index);
    //引数で指定したパッドの左スティックの正規化したY軸を取得
    float GetLeftSthikY(int index);
    //引数で指定したパッドの右スティックの正規化したX軸を取得
    float GetRightSthikX(int index);
    //引数で指定したパッドの右スティックの正規化したY軸を取得
    float GetRightSthikY(int index);
    
    //引数で指定したパッドが接続されているかを取得
    bool IsConnect(int index) { return m_pInputs[index]->IsConnect(); }

    //別の場所に追加するべき----------
    PlayerSlot& GetSlot(int index) { return m_Slot[index]; }

    //com作る時にもっと練って作り直しておく
    void ChangeSlot(int index)
    {
        if (m_Slot[index].ready)
        {
            m_Slot[index].ready = false;
        }
        else
        {
            m_Slot[index].ready = true;
        }
    }
    //CInputのものではない
    void SetHuman(int index) 
    {
        m_Slot[index].state = PlayerSlotState::Player;
        m_Slot[index].ready = true;
    }
    //CInputのものではない
    void SetCPU(int index) 
    {
        m_Slot[index].state = PlayerSlotState::COM;
        m_Slot[index].ready = false;
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