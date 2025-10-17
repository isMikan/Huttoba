#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

#include "PlayerObserver/IPlayerObserver.h"

#include "PlayerHead/CPlayerHead.h"
#include "PlayerHand/PlayerRightHand/CPlayerRightHand.h"
#include "PlayerHand/PlayerLeftHand/CPlayerLeftHand.h"

#include "PlayerState/CPlayerState.h"

/**************************************************
*   プレイヤーベースクラス.
**/
class CPlayerBase
	: public CStaticMeshObject
{
public:
	//プレイヤーの状態イベント.
	enum class PlayerEvent
	{
		Idle,
		Pickup,
		Throw,
		HandAttack,
		HandHit,
		HandWhiff,
		Pushed,
		Knockback,
		Falling,
		Knockdown,
		Getup,

		None = -1,
	};

	//接触イベント.
	enum class HitEvent
	{
		Pushed,			//押し出し.
		Knockback,		//吹き飛ばし.
		Down,			//ダウン.

		None = -1		//なし(攻撃側の接触).
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
		D3DXVECTOR3		position;					//攻撃された位置.
		D3DXVECTOR3		velocity;					//初速度.
		float			force = 0.f;				//攻撃力.
		bool			isHit = false;				//攻撃を受けたか.
		HitEvent		hitEvent = HitEvent::None;	//アニメーション.
	};

public:
	CPlayerBase( int index );
	virtual ~CPlayerBase();

	//--- 更新処理 ---.
	virtual void Update() override;
	//--- 描画処理 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//--- オブサーバを追加 ---.
	void AddObserver(IPlayerObserver* observer);

	//--- オブサーバを削除 ---.
	void RemoveObserver(IPlayerObserver* observer);

	//--- オブサーバに通知する ---.
	void Notify(IPlayerObserver::PlayerEvent event);

	//--- 移動状態を設定する関数 ---.
	void SetMoveState(std::unique_ptr<CPlayerState> newState);
	//--- 回転状態を設定する関数 ---.
	void SetTurnState(std::unique_ptr<CPlayerState> newState);
	//--- 行動状態を設定する関数 ---.
	void SetActionState(std::unique_ptr<CPlayerState> newState);

	//--- 位置を設定するために計算する関数 ---.
	D3DXVECTOR3 GetObjectPos(D3DXVECTOR3 offset);

	//--- ローカル座標軸を取得する関数 ---.
	LocalAxes GetLocalAxes();

	//--- クォータニオンによるベクトル回転の関数 ---.
	D3DXVECTOR3 RotateVectorByQuat(
		D3DXVECTOR3 vector, D3DXQUATERNION quat);

	//--- プレイヤーの初期角度から傾きを計算する関数 ---.
	D3DXQUATERNION TiltedQuat(
		D3DXQUATERNION	baseQuat,	//基準にする回転姿勢.
		D3DXVECTOR3		localAxes,	//ローカル軸の方向.
		float			tiltAngle);	//傾きの角度.

	//--- 押された時の移動量を計算する関数 ---.
	D3DXVECTOR3 Pushed();

	//--- 攻撃を受けた時のの移動量 ---.
	D3DXVECTOR3 GetVelocity();

	//--- 数値の領域を指定する関数 ---.
	float Clamp(float value, float min, float max);

	//--- 角度を0～360度にする関数 ---.
	float WrapAngle(float value);

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

	//アイテムを所持しているかの所得と設定.
	bool IsHoldingItem() const { return m_IsHoldingItem; }
	void SetHoldingItem(bool holding) { m_IsHoldingItem = holding; }

	//プレイヤーのイベントの所得と設定.
	PlayerEvent GetPlayerEvent() const { return m_PlayerEvent; }
	void SetPlayerEvent(PlayerEvent state) { m_PlayerEvent = state; }
	
	//攻撃を受けた情報を取得と設定.
	HitInfo GetHitInfo() const { return m_HitInfo; }
	//状況を設定用.
	void SetHitInfo(
		bool isHit, HitEvent anim)
	{
		m_HitInfo.isHit = isHit;
		m_HitInfo.hitEvent = anim;
	}
	//押し出し用.
	void SetHitInfo(
		D3DXVECTOR3 pos, bool isHit, HitEvent anim)
	{
		m_HitInfo.position = pos;
		m_HitInfo.isHit = isHit;
		m_HitInfo.hitEvent = anim;
	}
	//吹き飛ばし用.
	void SetHitInfo(
		D3DXVECTOR3 pos, D3DXVECTOR3 velocity,
		float force, bool isHit, HitEvent anim)
	{
		m_HitInfo.position = pos;
		m_HitInfo.velocity = velocity;
		m_HitInfo.force = force;	// 7 ～ 15 推奨.
		m_HitInfo.isHit = isHit;
		m_HitInfo.hitEvent = anim;
	}

protected:
	//--- 状態を変更を処理する関数 ---.
	void ChangeState(
		std::unique_ptr<CPlayerState>& currentState,
		std::unique_ptr<CPlayerState> newScene);

protected:
	std::vector<IPlayerObserver*>		m_pObserver;	//プレイヤーのオブサーバ.

	std::unique_ptr<CPlayerHead>		m_pHead;		//頭.
	std::unique_ptr<CPlayerRightHand>	m_pRightHand;	//右手.
	std::unique_ptr<CPlayerLeftHand>	m_pLeftHand;	//左手.

	std::unique_ptr<CPlayerState>	m_pMoveState;		//移動.
	std::unique_ptr<CPlayerState>	m_pTurnState;		//回転.
	std::unique_ptr<CPlayerState>	m_pActionState;		//行動.

	bool			m_IsMoving;		//移動しているか.
	bool			m_IsRotating;	//回転しているか.
	bool			m_IsHoldingItem;//アイテムを所持してるか.

	PlayerEvent		m_PlayerEvent;	//プレイヤー状態.
	HitInfo			m_HitInfo;		//攻撃を受けた情報.

	static constexpr float		m_PushForce = 0.05f;	//押し出す力.
};