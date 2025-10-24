#pragma once

#include "ItemFactory/ItemFactory.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"


class CPlayerManager;

class ItemBase :
	public CStaticMeshObject
	
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
	static bool AutoRegister(const std::string& itemId)
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
	virtual void Update(std::unique_ptr<CPlayerManager>& playiers);

	//継承が邪魔になったので切る。突貫工事
	virtual void Update() override;


	/*****************************************************************************************
	* @brief    描画処理
	*****************************************************************************************/
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override = 0;


	/*****************************************************************************************
	* @brief    プレイヤーインスタンス取得する
	* @param    player : プレイヤーのインスタンス
	*****************************************************************************************/
	void GetPlayer(CPlayerManager* player) { m_pPlayer = player; }

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


protected:

	virtual void Spawn	 ()	= 0; // 出現
	virtual void OnGround()	= 0; // 取得待機
	virtual void Have	 (std::unique_ptr<CPlayerManager>& playiers)	= 0; // 取得
	virtual void Use	 (std::unique_ptr<CPlayerManager>& playiers)	= 0; // 使用
	virtual void Throw	 (std::unique_ptr<CPlayerManager>& playiers)	= 0; // 投擲
	virtual void Destroy ()	= 0; // 消滅

protected:

	float m_tGravity;	// 重力加速度(簡易版)
	bool m_IsDestroy;	// 破壊するかどうか

	State m_State;		// アイテムの状態

	CPlayerManager* m_pPlayer; // 持っているプレイヤーのポインタ(プレイヤーの座標取得に必要)


};