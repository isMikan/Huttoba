#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/ActionState/CActionState.h"

/**************************************************
*	プレイヤーが何も行動していない状態クラス.
**/
class CPlayerActionIdle
	: public CActionState	//行動状態のクラスの継承.
{
public:
	CPlayerActionIdle();
	~CPlayerActionIdle() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:

};