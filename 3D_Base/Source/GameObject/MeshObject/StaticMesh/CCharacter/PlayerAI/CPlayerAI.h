#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CCharacter.h"

#include "PlayerHead/CPlayerHead.h"
#include "PlayerHand/PlayerRightHand/CPlayerRightHand.h"
#include "PlayerHand/PlayerLeftHand/CPlayerLeftHand.h"

#include "PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayerAI
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	//攻撃を受けたアニメーションパターン.
	enum class StatePattern
	{
		Push,			//押し出し.
		Knockback,		//吹き飛ばし.
		Down,			//ダウン.
		None = -1		//なし.
	};

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
		D3DXVECTOR3	position;	//攻撃された位置.
		D3DXVECTOR3 velocity;	//初速度.
		float		force;		//攻撃力.
		bool		isHit;		//攻撃を受けたか.
		StatePattern	animName;	//アニメーション.
	};

public:
	CPlayerAI(int index);		//引数はコントローラ番号.
	~CPlayerAI() override;

	//----- 毎フレームの動作する関数 -----.
	void Update() override;
	//----- 毎フレームの描画する関数 -----.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//void AddObserver(IPlayerObserver* observer){
	//	observers.push_back(observer);
	//}
	// void Notify(PlayerEvent event){
	// for(auto& obs : observers){
	//		obs->OnNotify(event);
	//		}
	// }		 

	//----- キー操作 -----.
	void HandleInput();

	//----- 移動状態を設定する関数 -----.
	void SetMoveState(std::unique_ptr<CPlayerState> newState);
	//----- 回転状態を設定する関数 -----.
	void SetTurnState(std::unique_ptr<CPlayerState> newState);
	//----- 行動状態を設定する関数 -----.
	void SetActionState(std::unique_ptr<CPlayerState> newState);

	//----- 頭の位置を設定するために計算する関数 -----.
	D3DXVECTOR3 SetHeadPos();

	//----- プレイヤーの正面方向を取得するための関数 -----.
	D3DXVECTOR3 GetForward();

	//----- ローカル座標軸を取得する関数 -----.
	LocalAxes GetLocalAxes();

	//----- クォータニオンによるベクトル回転の関数 -----.
	D3DXVECTOR3 RotateVectorByQuat(
		D3DXVECTOR3 vector, D3DXQUATERNION quat);

	//----- プレイヤーの初期角度から傾きを計算する関数 -----.
	D3DXQUATERNION TiltedQuat(
		D3DXQUATERNION	baseQuat,	//基準にする回転姿勢.
		D3DXVECTOR3		localAxes,	//ローカル軸の方向.
		float			tiltAngle);	//傾きの角度.

	//----- 押された時の移動量を計算する関数 -----.
	D3DXVECTOR3 Knockback() const;

	//----- 攻撃を受けた時のの移動量 -----.
	D3DXVECTOR3 GetVelocity() const;

	//----- 数値の領域を指定する関数 -----.
	float Clamp(float value, float min, float max);

	//----- 角度を0～360度にする関数 -----.
	float WrapAngle(float value);

	//----- キャラクターの色を設定する関数 -----.
	void SetCharacterDefault(int index);

	//----- プレイヤーが頭を持っている(書き込み用) -----.
	CPlayerHead& GetPlayerHead() { return *m_pHead; }
	//----- プレイヤーが頭を持っている(読み込み用) -----.
	const CPlayerHead& GetPlayerHead() const { return *m_pHead; }

	//----- プレイヤーが右手を持っている(書き込み用) -----.
	CPlayerRightHand& GetPlayerRightHand() { return *m_pRightHand; }
	//----- プレイヤーが右手を持っている(読み込み用) -----.
	const CPlayerRightHand& GetPlayerRightHand() const { return *m_pRightHand; }

	//----- プレイヤーが左手を持っている(書き込み用) -----.
	CPlayerLeftHand& GetPlayerLeftHand() { return *m_pLeftHand; }
	//----- プレイヤーが左手を持っている(読み込み用) -----.
	const CPlayerLeftHand& GetPlayerLeftHand() const { return *m_pLeftHand; }

	//----- 移動しているかの所得と設定 -----.
	bool IsMoving() const { return m_IsMoving; }
	void SetMoving(bool moving) { m_IsMoving = moving; }

	//----- 回転しているかの所得と設定 -----.
	bool IsRotating() const { return m_IsRotating; }
	void SetRotating(bool rotating) { m_IsRotating = rotating; }

	//----- アイテムを手に入れているかの取得と設定 -----.
	bool IsHoldingItem() const { return m_IsHoldingItem; }
	void SetHoldingItem(bool holdingItem) { m_IsHoldingItem = holdingItem; }

	//----- 攻撃しているかの取得と設定 -----.
	bool IsAttacking() const { return m_IsAttacking; }
	void SetAttacking(bool attacking) { m_IsAttacking = attacking; }

	//----- 攻撃しているかの取得と設定 -----.
	bool IsStopping() const { return m_IsStopping; }
	void SetStopping(bool stopping) { m_IsStopping = stopping; }

	//----- 攻撃を受けた情報を取得と設定 -----.
	HitInfo GetHitInfo() const { return m_HitInfo; }
	void SetHitInfo(
		bool isHit, StatePattern anim)
	{
		m_HitInfo.isHit = isHit;
		m_HitInfo.animName = anim;
	}
	void SetHitInfo(
		D3DXVECTOR3 pos, D3DXVECTOR3 velocity,
		float force, bool isHit, StatePattern anim)
	{
		m_HitInfo.position = pos;
		m_HitInfo.velocity = velocity;
		m_HitInfo.force = force;	//7 以上、15 以下推奨.
		m_HitInfo.isHit = isHit;
		m_HitInfo.animName = anim;
	}

private:
	//----- キーバインドを設定する関数 -----.
	void SetPlayerInputBinding(int index);
	//----- 状態を変更を処理する関数 -----.
	void ChangeState(
		std::unique_ptr<CPlayerState>& currentState,
		std::unique_ptr<CPlayerState> newScene);

private:
	std::unique_ptr<CInput>				m_pInput;		//入力.

	std::unique_ptr<CPlayerHead>		m_pHead;		//頭.
	std::unique_ptr<CPlayerRightHand>	m_pRightHand;	//右手.
	std::unique_ptr<CPlayerLeftHand>	m_pLeftHand;	//左手.

	std::unique_ptr<CPlayerState>	m_pMoveState;		//移動.
	std::unique_ptr<CPlayerState>	m_pTurnState;		//回転.
	std::unique_ptr<CPlayerState>	m_pActionState;		//行動.

	bool		m_IsMoving;			//移動しているか.
	bool		m_IsRotating;		//回転しているか.
	bool		m_IsHoldingItem;	//アイテムを持っているか.
	bool		m_IsAttacking;		//攻撃しているか.
	bool		m_IsStopping;		//止まるとき.

	HitInfo		m_HitInfo;			//攻撃を受けた情報.
};