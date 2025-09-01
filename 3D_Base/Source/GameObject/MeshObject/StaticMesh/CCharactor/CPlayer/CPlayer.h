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
	//ローカル軸の構造体.
	struct LocalAxes
	{
		D3DXVECTOR3 right;		//ローカルX軸(右).
		D3DXVECTOR3 up;			//ローカルY軸(上).
		D3DXVECTOR3 forward;	//ローカルZ軸(正面).
	};

public:
	CPlayer();
	~CPlayer() override;

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	void HandleInput();

	//移動状態を設定する関数.
	void SetMoveState(std::unique_ptr<CPlayerDirectionalInputState> newState);
	//回転状態を設定する関数.
	void SetRotationState(std::unique_ptr<CPlayerDirectionalInputState> newState);
	//行動状態を設定する関数.
	void SetActionState(std::unique_ptr<CActionState> newState);

	//プレイヤーの正面方向を取得するための関数.
	D3DXVECTOR3 GetForward();

	//ローカル座標軸を取得する関数.
	LocalAxes GetLocalAxes();

	//プレイヤーの初期角度から傾きを計算する関数..
	D3DXQUATERNION TiltedQuat(
		D3DXQUATERNION	baseQuat,	//基準にする回転姿勢.
		D3DXVECTOR3		localAxes,	//ローカル軸の方向.
		float			tiltAngle);	//傾きの角度.

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

private:
	//キーバインドを設定する関数.
	void SetPlayerInput();

	//テンプレート関数(中身の処理は同じもの).
	template<typename StateType>
	//状態遷移の処理関数.
	void ChangeState(
		std::unique_ptr<StateType>& currentState,
		std::unique_ptr<StateType> newScene);

private:
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

//型が決まっていないのでここで定義.
//inline 複数定義されても毎回インスタンスを生成しない.
template<typename StateType>
inline void CPlayer::ChangeState(
	std::unique_ptr<StateType>& currentState,
	std::unique_ptr<StateType> newState)
{
	if (currentState != nullptr)
	{
		//状態の終了処理.
		currentState->Exit(*this);
	}

	//新しい状態にする.
	currentState = std::move(newState);

	if (currentState != nullptr)
	{
		//状態の開始処理.
		currentState->Enter(*this);
	}
}
