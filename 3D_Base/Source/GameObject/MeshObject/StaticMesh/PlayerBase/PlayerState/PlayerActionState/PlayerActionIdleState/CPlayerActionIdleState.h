#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが何も行動していない状態クラス.
**/
class CPlayerActionIdleState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerActionIdleState();
	~CPlayerActionIdleState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayerBase& pPlayerBase) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayerBase& pPlayerBase) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayerBase& pPlayerBase) override;

private:

};