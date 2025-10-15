#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが回転する状態クラス.
**/
class CPlayerTurnState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerTurnState(float x, float z);	//引数:回転方向のx軸、z軸.
	~CPlayerTurnState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayerBase& pPlayerBase) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayerBase& pPlayerBase) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayerBase& pPlayerBase) override;

private:
	D3DXVECTOR3		m_Dir;			//方向キーの入力.

	float			m_TurnSpeed;	//回転速度.
	float			m_TurnDir;		//目的の方向.

private:
	CPlayerTurnState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerTurnState(const CPlayerTurnState&) = delete;
	CPlayerTurnState& operator = (const CPlayerTurnState& rhs) = delete;

};