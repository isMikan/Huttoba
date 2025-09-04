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

    void Update();

    CInput& GetInput(int index) { return *m_pInputs[index]; }

    void BindKey(Action action, const InputBinding& binding);

    void BindKey(Action action, const InputBinding& binding, int i);


    //別の場所に追加するべき-----------
    PlayerSlot& GetSlot(int index) { return m_Slot[index]; }

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

    void SetHuman(int index) 
    {
        m_Slot[index].state = PlayerSlotState::Player;
        m_Slot[index].ready = true;
    }

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
    std::array<std::unique_ptr<CInput>, 4> m_pInputs;

    std::array<PlayerSlot, 4>                m_Slot;    //仮置き
};