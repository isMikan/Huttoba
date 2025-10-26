#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが回転する状態クラス.
**/
class CPlayerTurnState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerTurnState(CPlayerBase& pPlayer, float x, float z);	//引数:回転方向のx軸、z軸.
	~CPlayerTurnState() override;

	//--- 状態の開始時に呼び出す ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す ---.
	void Exit() override;
	//--- この状態の間に呼び出す ---.
	void Update() override;

private:
	//--- 目的の方向に達成している場合 ---.
	bool IsSmallTurn(D3DXVECTOR3 forward);

private:
	D3DXVECTOR3		m_InputDir;		//入力を受けた方向.

	float			m_TurnSpeed;	//回転速度.
	float			m_TurnDir;		//回転する目標の方向.

private:
	CPlayerTurnState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerTurnState(const CPlayerTurnState&) = delete;
	CPlayerTurnState& operator = (const CPlayerTurnState& rhs) = delete;

};