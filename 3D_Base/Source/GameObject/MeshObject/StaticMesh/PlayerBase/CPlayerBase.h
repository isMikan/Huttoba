#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

#include "PlayerEventBus/CPlayerEventBus.h"

#include "PlayerHead/CPlayerHead.h"
#include "PlayerHand/PlayerRightHand/CPlayerRightHand.h"
#include "PlayerHand/PlayerLeftHand/CPlayerLeftHand.h"

#include "PlayerState/CPlayerState.h"

class ItemBase;

/***********************************************************************
*   プレイヤーベースクラス.						制作者 [甲把]
**/
class CPlayerBase
	: public CStaticMeshObject
	, public ICollisionListener
{
public:
//======================================================================
// 	   列挙型.
//======================================================================
	//動作する指示.
	enum class ActionInstruct
	{
		ToggleItem,		//アイテムを拾う・捨てる.
		Attack,			//攻撃.

		None = -1		//なし.
	};

	//接触イベント.
	enum class HitEvent
	{
	//=== 攻撃された側 ===.
		Pushback,	//押し出し攻撃.
		Knockback,	//吹き飛ばし攻撃.
		Knockdown,	//ダウン付き攻撃.

	//=== ダウン識別用 ===.
		WithDown,	//ダウンする.
		NoDown,		//ダウンしない.

	//=== 攻撃した側 ===.
		HandAttack,	//手の攻撃.


		None = -1	//なし(攻撃側の接触).
	};

//======================================================================
// 	   構造体.
//======================================================================
	//ローカル軸の情報.
	struct LocalAxes
	{
		D3DXVECTOR3 right;		//ローカルX軸(右).
		D3DXVECTOR3 up;			//ローカルY軸(上).
		D3DXVECTOR3 forward;	//ローカルZ軸(正面).
	};

	//攻撃を受けた情報.
	struct HitAttack
	{
		D3DXVECTOR3		velocity;					//初速度.
		HitEvent		hitEvent = HitEvent::None;	//アニメーション.
	};

	//他のプレイヤーに接触したときの情報.
	struct HitPlayer
	{
		D3DXVECTOR3	otherDir;	//接触したプレイヤーの方向.
		float		dot;		//角度差.
		bool		isHit;		//衝突したか.
	};

public:
	CPlayerBase( int index );
	virtual ~CPlayerBase();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	virtual void Update() override;
	//--- 描画処理 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//--- 移動状態を設定 ---.
	void SetMoveState(std::unique_ptr<CPlayerState> newState);
	//--- 回転状態を設定 ---.
	void SetTurnState(std::unique_ptr<CPlayerState> newState);
	//--- 行動状態を設定 ---.
	void SetActionState(std::unique_ptr<CPlayerState> newState);

	//--- 当たり判定生成 ---.
	void CreateCollider();

	//--- 地面との衝突判定 ---.
	void OnGroundCollision(CGroundManager &pGroundMgr);

	//--- 位置を設定するために計算 ---.
	D3DXVECTOR3 GetObjectPos(D3DXVECTOR3 offset);

	//--- ローカル座標軸を取得 ---.
	LocalAxes GetLocalAxes();

	//--- クォータニオンによるベクトル回転 ---.
	D3DXVECTOR3 RotateVectorByQuat(
		D3DXVECTOR3 vector, D3DXQUATERNION quat);

	//--- プレイヤーの初期角度から傾きを計算 ---.
	D3DXQUATERNION TiltedQuat(
		D3DXQUATERNION	baseQuat,	//基準にする回転姿勢.
		D3DXVECTOR3		localAxes,	//ローカル軸の方向.
		float			tiltAngle);	//傾きの角度.

	//--- 押された時の移動量を計算 ---.
	D3DXVECTOR3 GetPushbackVelocity(D3DXVECTOR3 sourcePos);	

	//--- 攻撃を受けた時のの移動量を計算 ---.
	D3DXVECTOR3 GetKnockbackVelocity(
		D3DXVECTOR3 sourcePos,	//攻撃を受けた起源.
		float power,			//吹き飛び量.
		float angle);			//角度.

	//--- 角度を0～360度にする ---.
	float WrapAngle(float value);

//======================================================================

	//当たり判定削除をマネージャーで行うので取得できるようにする
	std::shared_ptr<CollisionBase> GetCollider() { return m_pCollision; }

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

	//持っているアイテムを取得と設定.
	ItemBase* GetItemBase() const { return m_pItemBase; }
	void SetItemBase(ItemBase* item) { m_pItemBase = item; }

	//プレイヤー番号を取得.
	int GetPlayerID() const { return m_PlayerID; }

	//攻撃を受けた力を取得.
	float GetHitPower() const { return m_HitForce; }
	
	//攻撃を受けた情報を取得と設定.
	HitAttack GetHitAttack() const { return m_HitAttack; }
	//アニメーション状態変更用.
	void SetHitAnim(HitEvent anim) {
		m_HitAttack.hitEvent = anim;		//ヒットアニメーション.
	}
	//攻撃情報設定用.
	void SetHitAttack(
		D3DXVECTOR3 velocity, HitEvent anim){
		m_HitAttack.velocity = velocity;	//移動量（Get [Pushback・Knockback] Velocityを入れてください）.
		m_HitAttack.hitEvent = anim;		//ヒットアニメーション.
	}

	//プレイヤーの接触情報を取得と設定.
	HitPlayer GetHitPlayer() const { return m_HitPlayer; }

	//ダウン状態の時間を取得と設定.
	Gauge GetKnockdownTime() const { return m_KnockdownTime; }
	void SetKnockdownTime(float remaining, float max) {
		m_KnockdownTime.remaining = remaining; 
		m_KnockdownTime.max = max; 
	}

	//移動しているかの所得と設定.
	bool IsMoving() const { return m_IsMoving; }
	void SetMoving(bool moving) { m_IsMoving = moving; }

	//回転しているかの所得と設定.
	bool IsTurning() const { return m_IsTurning; }
	void SetTurning(bool turning) { m_IsTurning = turning; }

	//作成者　佐藤
	//地面についているかの所得と設定.
	bool IsOnGround() const { return m_IsOnGround; }
	void SetIsOnGround(bool isOnGround) { m_IsOnGround = isOnGround; }

	//ステージの上かの所得と設定.
	bool IsAboveGround() const { return m_IsAboveGround; }
	void SetIsAboveGround(bool isAboveGround) { m_IsAboveGround = isAboveGround; }

	//nullptr ではないかチェック.
	template<typename T>
	bool IsActionState() const {
		return (m_pActionState
			&& dynamic_cast<T*>(m_pActionState.get()) != nullptr);
	}
	//複数のテンプレート.
	template<typename... Ts>
	//複数の条件に対応して結果を返す.
	bool IsAnyActionState() const { 
		return ( ... || IsActionState<Ts>());	//... 条件にしたいStateを入れる.
	}

	//イベントバスを持つ.
	CPlayerEventBus<CPlayerState>& GetBus() { return m_Bus; }

protected:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 状態を変更を処理する ---.
	void ChangeState(
		std::unique_ptr<CPlayerState>& currentState,
		std::unique_ptr<CPlayerState> newScene);

	//--- 衝突判定 ---.
	void OnCollision(CollisionBase* pOtherCollider) override;

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	int		m_PlayerID;		//プレイヤー番号.

	CPlayerEventBus<CPlayerState>		m_Bus;			//プレイヤー状態.

	std::unique_ptr<CPlayerHead>		m_pHead;		//頭.
	std::unique_ptr<CPlayerRightHand>	m_pRightHand;	//右手.
	std::unique_ptr<CPlayerLeftHand>	m_pLeftHand;	//左手.

	std::unique_ptr<CPlayerState>		m_pMoveState;	//移動.
	std::unique_ptr<CPlayerState>		m_pTurnState;	//回転.
	std::unique_ptr<CPlayerState>		m_pActionState;	//行動.

	ItemBase*		m_pItemBase;		//アイテムベース（ここに所持アイテムを入れる）.

	ActionInstruct	m_Control;			//指示.
	HitAttack		m_HitAttack;		//攻撃を受けた情報.
	HitPlayer		m_HitPlayer;		//プレイヤーの接触情報.
	Gauge			m_KnockdownTime;	//ダウン状態の時間を保存.

	bool			m_IsMoving;			//移動しているか.
	bool			m_IsTurning;		//回転しているか.

	bool			m_IsOnGround;		//地面に接触しているか
	bool			m_IsAboveGround;	//ステージの上かどうか

	float			m_HitForce;			//攻撃を受けた数値.
	static constexpr float		m_PushForce = 0.05f;	//押し出す力.
};