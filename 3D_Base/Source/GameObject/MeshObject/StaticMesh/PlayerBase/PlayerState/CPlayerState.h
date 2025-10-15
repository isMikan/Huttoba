#pragma once

#include <string>

class CPlayerBase;

/****************************************
*	プレイヤー状態継承クラス.
**/
class CPlayerState
{
public:
	CPlayerState() {};
	virtual ~CPlayerState() {};

	//状態が始めるときだけ呼び出される.
	virtual void Enter(CPlayerBase& pPlayerBase) = 0;
	//状態が終わるときだけ呼び出される.
	virtual void Exit(CPlayerBase& pPlayerBase) = 0;
	//この状態の間は毎フレーム呼び出される.
	virtual void Update(CPlayerBase& pPlayerBase) = 0;

protected:

};