#pragma once
#include "CInput.h"
#include <array>

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

    //第2引数で指定したInputのIsDown(押した)
    static bool IsDown(Action action, int index);
    
    //第2引数で指定したInputのIsUp(離した)
    static bool IsUp(Action action, int index);

    //第2引数で指定したInputのIsRepeat(押し続けているか)
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

    //レバガチャ用のボタンを入力したかを渡す.
    static bool IsDownKey(int index) { return  Instance().m_pInputs[index]->IsDownKey(); }

    //レバガチャ成功時のコントローラーの振動
   // static void VibrationController(int index) { Instance().m_pInputs[index]->SetVibration(); }

private:
    CInputManager();
    CInputManager(const CInputManager&) = delete;
    CInputManager& operator=(const CInputManager&) = delete;

private:
    std::array<std::unique_ptr<CInput>, 4>  m_pInputs;
};