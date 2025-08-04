#pragma once
#include "CInput.h"
#include <array>

//準備画面でのプレイヤーの状態.
enum class PlayerSlotState
{
	None,
	Player,
	COM,

	Max
};

//プレイヤーの情報をまとめる.
struct PlayerSlot
{
	PlayerSlotState state;	//準備画面でのプレイヤーの状態.
	int inputIndex;			//対応するコントローラーの番号.
};

class CInputManager
{
public:
	static CInputManager& GetInstance()
	{
		static CInputManager instance;
		return instance;
	}

private:
	CInputManager()/* = default*/;
	~CInputManager();

public:
	//void Initialize();
	void Update();

	//プレイヤーの参加管理.
	void JoinPlayer(int inputIndex);

	CInput& GetPlayerIndex(int playerIndex);

private:

	CInputManager(const CInputManager&) = delete;
	CInputManager& operator=(const CInputManager&) = delete;

	//m_Inputの作成.
	void InitializeInput();

private:
	//ゲームのプレイヤーの最大人数Globalに入れるべきかも.
	static const int PlayerMax = 4;

private:
	//プレイヤーの数だけCInput作成する.
	std::array<std::unique_ptr<CInput>, PlayerMax>	m_Inputs;	

	//プレイヤーが参加するかをとる.
	std::array < PlayerSlot, PlayerMax>					m_PlayerSlot;
};