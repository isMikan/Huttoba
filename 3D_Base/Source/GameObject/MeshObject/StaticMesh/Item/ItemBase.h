#pragma once

#include "ItemFactory/ItemFactory.h"
#include "CStaticMeshObject.h"
#include "PlayerBase/Player/CPlayer.h"
#include "Collision/CollisionDraw/CollisionDraw.h"

class ItemBase :
	public CStaticMeshObject,public ICollisionListener
	
{
public:
	ItemBase();
	~ItemBase();

	/*メモ
	アイテムのゲージなどの情報をゲージマネージャーに渡すためのメソッドが必要
	耐久地や残り時間・現在地などの情報が必要。
	
	BaseにデータとGet関数を用意し、各子クラスで定義。
	Get関数でManagerから引き出せるように。
	*/

	/*****************************************************************************************
	* @brief    アイテムを登録するためのテンプレート関数
	* @param    登録したいアイテムのID
	* @return   bit数が少ないboolを利用しているだけで、引数に意味はない
	*****************************************************************************************/

	template<typename T>
	static bool AutoRegister(const ItemID itemId)
	{
		ItemFactory::GetInstance()->RegisterItem(itemId, []()
			{
				return std::make_unique<T>();
			});
		return true;
	}
	
	// アイテムの状態
	enum class State
	{
		None = -1,
		Spawn,
		OnGround,
		Have,
		Use,
		Throw,
		Destroy,
	};


	/*****************************************************************************************
	* @brief    初期化
	******************************************************************************************/
	virtual void Init() = 0; // 


	/*****************************************************************************************
	* @brief    更新処理
	*****************************************************************************************/
	virtual void Update() override;


	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override = 0;


	/*****************************************************************************************
	* @brief    プレイヤーインスタンス取得する
	* @param    player : プレイヤーのインスタンス
	*****************************************************************************************/
	CPlayerBase* GetPlayer() { return m_pPlayer; }

	/*****************************************************************************************
	* @brief    プレイヤーインスタンスセットする
	* @param    player : プレイヤーのインスタンス
	*****************************************************************************************/
	void SetPlayer(CPlayerBase* player) { m_pPlayer = player; }


	/*****************************************************************************************
	* @brief    アイテムの状態を変更するクラス
	* @param    state : アイテムの状態を管理する変数
	*****************************************************************************************/
	void SetState(State state) { m_State = state; }


	/*****************************************************************************************
	* @brief    アイテム使用中かどうか
	* @return	true : 使用している
	*			false: 使用していない
	*****************************************************************************************/
	bool IsUse(); 


	/*****************************************************************************************
	* @brief	m_IsDestroyのがtrueならインスタンスを削除する
	* @deltase	アイテム基底クラスでm_IsDestroyが宣言されている
	*			各アイテム子クラスのDestoroy関数でm_IsDestroyのフラッグをtrueにする
	* @return   true : アイテム削除
	*			false: なにもしない
	* 
	*****************************************************************************************/
	bool IsDestroy() { return m_IsDestroy; }

	//地面との判定
	void IsOnGround(CGroundManager& pGroundMgr);

	bool GetIsOnGround() { return m_IsOnGround; }
	bool GetIsUse()   { return m_IsUse;	}

	void Fall();

	//アイテム消去(他クラスで読み込む用)
	void DestroyItem();

	//当たり判定処理
	virtual void OnCollision(CollisionBase* other)override;


protected:

	virtual void Spawn	 ()	= 0; // 出現
	virtual void OnGround()	= 0; // 取得待機
	virtual void Have	 ()	= 0; // 取得
	virtual void Use	 ()	= 0; // 使用
	virtual void Throw	 ()	= 0; // 投擲
	virtual void Destroy ()	= 0; // 消滅

	virtual void ChangeState(State state) = 0;	//状態変化時に一度だけ処理

protected:

	float m_tGravity;	// 重力加速度(簡易版)
	float m_UseTime;	// 使用時間
	int  m_UseCount;	// 使用回数
	bool m_IsDestroy;	// 破壊するかどうか
	bool m_IsOnGround;	// 地面に接触しているか
	bool m_IsUse;		// 使用中か

	State m_State;		// アイテムの状態
	State m_OldState;	// アイテムの前の状態

	CPlayerBase* m_pPlayer; //当たり判定で接触したPlayerを入れるポインタ	

	//当たり判定(判定の削除にポインタが必要なので保持)
	std::shared_ptr<CollisionBase>	m_pPickUpCollider;	//拾う用当たり判定
	std::shared_ptr<CollisionBase>	m_pUseCollider;		//使用中用当たり判定

};