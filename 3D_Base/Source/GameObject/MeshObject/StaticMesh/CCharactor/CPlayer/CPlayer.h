#pragma once

#include "CInput/CInput.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"

#include "PlayerHand/PlayerRightHand/CPlayerRightHand.h"
#include "PlayerHand/PlayerLeftHand/CPlayerLeftHand.h"

#include "PlayerState/DirectionalInputState/CPlayerDirectionalInputState.h"
#include "PlayerState/ActionState/CActionState.h"

/**************************************************
*	サイズ後々消す(メタセコイアでアイテム作るとき参考にしてください)
* 
*	プレイヤー	頭0.3
*				体1.0
*				手0.15
/**************************************************


/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	CPlayer();
	~CPlayer() override;

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;


	void HandleInput();

	void SetMoveState(std::unique_ptr< CPlayerDirectionalInputState> newState);
	void SetRotationState(std::unique_ptr< CPlayerDirectionalInputState> newState);
	void SetActionState(std::unique_ptr<CActionState> newState);

	//キーバインドの設定.
	void SetPlayerInput();

	//プレイヤーの正面方向を取得.
	D3DXVECTOR3 GetForward();

	//プレイヤーの方向から位置計算の関数.
	D3DXVECTOR3 HandPositionMath(D3DXVECTOR3 offsetPos);

	//プレイヤーが右手を持っている(書き込み用).
	CPlayerRightHand& GetPlayerRightHand() { return *m_pRightHand; }
	//プレイヤーが右手を持っている(読み込み用).
	const CPlayerRightHand& GetPlayerRightHand() const { return *m_pRightHand; }

	//プレイヤーが左手を持っている(書き込み用).
	CPlayerLeftHand& GetPlayerLeftHand() { return *m_pLeftHand; }
	//プレイヤーが左手を持っている(読み込み用).
	const CPlayerLeftHand& GetPlayerLeftHand() const { return *m_pLeftHand; }

	//移動しているかの所得と設定.
	bool IsMoving() const { return m_IsMoving; }
	void SetMoving(bool moving) { m_IsMoving = moving; }

	//回転しているかの所得と設定.
	bool IsRotating() const { return m_IsRotating; }
	void SetRotating(bool rotating) { m_IsRotating = rotating; }

protected:
	std::unique_ptr<CInput>	m_pInput;	//入力.

	std::unique_ptr<CPlayerRightHand>	m_pRightHand;		//右手.
	std::unique_ptr<CPlayerLeftHand>	m_pLeftHand;		//左手.

	std::unique_ptr<CPlayerDirectionalInputState>	m_pMoveState;		//移動.
	std::unique_ptr<CPlayerDirectionalInputState>	m_pRotationState;	//回転.
	std::unique_ptr<CActionState>					m_pActionState;		//行動.

	D3DXVECTOR3 m_Forward;		//正面方向.
	D3DXVECTOR3 m_Velocity;
	bool		m_IsMoving;		//移動しているか.
	bool		m_IsRotating;	//回転しているか.
	bool		m_IsBlown;
};