#pragma once


class CPlayerBase;

/****************************************
*	プレイヤー状態継承クラス.
**/
class CPlayerState
{
public:
	CPlayerState(CPlayerBase& pPlayer);
	virtual ~CPlayerState() = default;

	//--- 状態の開始時に呼び出す ---.
	virtual void Enter() = 0;
	//--- 状態の終了時に呼び出す ---.
	virtual void Exit() = 0;
	//--- この状態の間に呼び出す ---.
	virtual void Update() = 0;

protected:
	CPlayerBase&	m_pPlayer;		//プレイヤーを参照.

protected:
	CPlayerState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerState(const CPlayerState&) = delete;
	CPlayerState& operator = (const CPlayerState& rhs) = delete;

};