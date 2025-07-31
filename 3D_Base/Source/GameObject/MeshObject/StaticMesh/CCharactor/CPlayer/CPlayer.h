#pragma once

#include "CInput/CInput.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"
#include "PlayerState/CPlayerMoveState.h"
#include "PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	//移動状態列挙型.
	enum enMoveState
	{
		Stop = 0,	//停止.
		Forward,	//前進.
		Backward,	//後退.
		TurnLeft,	//左回転.
		TurnRight,	//右回転.
	};

public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	void BlownFrom(D3DXVECTOR3 position, float force);

	void HandleInput();

	void SetMoveState(std::unique_ptr< CPlayerMoveState> newState);
	void SetRotationState(std::unique_ptr< CPlayerMoveState> newState);

protected:
	std::unique_ptr<CInput>	m_pInput;		//入力.

	std::unique_ptr<CPlayerMoveState>	m_pMoveState;		//動き.
	std::unique_ptr<CPlayerMoveState>	m_pRotationState;	//回転.
	std::unique_ptr<CPlayerState>		m_pActionState;		//行動.

	D3DXVECTOR3 m_Velocity;
	bool		m_IsBlown;
};