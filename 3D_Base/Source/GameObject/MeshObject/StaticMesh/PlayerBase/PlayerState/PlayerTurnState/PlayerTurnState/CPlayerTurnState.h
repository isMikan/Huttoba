#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが回転する状態クラス.
**/
class CPlayerTurnState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerTurnState(CPlayerBase& pPlayer, float x, float z);	//引数:回転方向のx軸、z軸.
	~CPlayerTurnState() override;

	//--- 通知 ---.
	void OnNotify(IPlayerObserver::PlayerEvent event) override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	D3DXVECTOR3		m_Dir;			//方向キーの入力.

	float			m_TurnSpeed;	//回転速度.
	float			m_TurnDir;		//目的の方向.

private:
	CPlayerTurnState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerTurnState(const CPlayerTurnState&) = delete;
	CPlayerTurnState& operator = (const CPlayerTurnState& rhs) = delete;

};