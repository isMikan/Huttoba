#pragma once

#include "CInput/CInput.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"

#include "PlayerHead/CPlayerHead.h"
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

	//攻撃を受けた情報.
	struct HitInfo
	{
		D3DXVECTOR3 position;		//攻撃された位置.
		float		force;			//攻撃力.
		bool		isHit;			//攻撃を受けたか.
	};

public:
	CPlayer(int index);	//引数はコントローラ番号.
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

	//頭の位置を設定するために計算する関数.
	D3DXVECTOR3 SetHandPos();

	//プレイヤーの正面方向を取得するための関数.
	D3DXVECTOR3 GetForward();

	//ローカル座標軸を取得する関数.
	LocalAxes GetLocalAxes();

	//プレイヤーの初期角度から傾きを計算する関数..
	D3DXQUATERNION TiltedQuat(
		D3DXQUATERNION	baseQuat,	//基準にする回転姿勢.
		D3DXVECTOR3		localAxes,	//ローカル軸の方向.
		float			tiltAngle);	//傾きの角度.

	//押された時の移動量を計算する関数.
	D3DXVECTOR3 Knockback();

	//キャラクターの色を設定する関数.
	void SetCharacterDefault(int index);

	//プレイヤーが頭を持っている(書き込み用).
	CPlayerHead& GetPlayerHead() { return *m_pHead; }
	//プレイヤーが頭を持っている(読み込み用).
	const CPlayerHead& GetPlayerHead() const { return *m_pHead; }

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

	//アイテムを手に入れているかの取得と設定.
	bool IsHoldingItem() const { return m_IsHoldingItem; }
	void SetHoldingItem(bool holdingItem) { m_IsHoldingItem = holdingItem; }

	//攻撃しているかの取得と設定.
	bool IsAttacking() const { return m_IsAttacking; }
	void SetAttacking(bool attacking) { m_IsAttacking = attacking; }

	//攻撃を受けた情報を取得と設定.
	HitInfo GetHitInfo() const { return m_HitInfo; }
	void SetHitInfo(D3DXVECTOR3 pos, float force, bool isHit) {
		m_HitInfo.position = pos;
		m_HitInfo.force = force;
		m_HitInfo.isHit = isHit;
	}

private:
	//キーバインドを設定する関数.
	void SetPlayerInputBinding();

	//テンプレート関数(中身の処理は同じもの).
	template<typename StateType>
	//状態遷移の処理関数.
	void ChangeState(
		std::unique_ptr<StateType>& currentState,
		std::unique_ptr<StateType> newScene);

private:
	std::unique_ptr<CInput>				m_pInput;		//入力.

	std::unique_ptr<CPlayerHead>		m_pHead;		//頭.
	std::unique_ptr<CPlayerRightHand>	m_pRightHand;	//右手.
	std::unique_ptr<CPlayerLeftHand>	m_pLeftHand;	//左手.

	std::unique_ptr<CPlayerDirectionalInputState>	m_pMoveState;		//移動.
	std::unique_ptr<CPlayerDirectionalInputState>	m_pRotationState;	//回転.
	std::unique_ptr<CActionState>					m_pActionState;		//行動.

	bool		m_IsMoving;			//移動しているか.
	bool		m_IsRotating;		//回転しているか.
	bool		m_IsHoldingItem;	//アイテムを持っているか.
	bool		m_IsAttacking;		//攻撃しているか.

	HitInfo		m_HitInfo;			//攻撃を受けた情報.
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
