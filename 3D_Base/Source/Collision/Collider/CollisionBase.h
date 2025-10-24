#pragma once

#include "Collision/CollisionListener/ICollisionListener.h"

/**
 * 衝突情報の基底クラス。
 * 責任: 衝突形状の定義、ワールド座標、親オブジェクトへの参照を保持。
 */
class CollisionBase
{
public:

    //オブジェクトタグ
    enum class ColliderTag
    {
        Player = 0,
        Item,
        Ground,
        Haetataki,
        SmashBat,
        Fan,
        Mushroom,
        Magnet,
        Boomerang,
        RatRobot,

        Max,
        None = -1
    };

    //形状タグ
    enum class ColliderType
    {
        Capsule = 0,
        OBB,
        Ray,
        Sphere,

        None = -1,
    };

    // コンストラクタ / デストラクタ
    CollisionBase(ICollisionListener* pListener,
        const D3DXVECTOR3& syncPosition,
        ColliderTag         tag,
        const D3DXVECTOR3& localOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f)
    );
    virtual ~CollisionBase() = default;

    /*****************************************************************************************
    * @brief     形状タイプを取得
    *****************************************************************************************/
    virtual ColliderType GetType() const = 0;


    /*****************************************************************************************
    * @brief     当たり判定の主を設定
    *****************************************************************************************/
    void SetListener(ICollisionListener* obj) { m_pListener = obj; }


    /*****************************************************************************************
    * @brief    当たり判定の主を取得
    * @return   当たり判定の主を
    *****************************************************************************************/
    ICollisionListener* GetListener() const { return m_pListener; }


    /*****************************************************************************************
    * @brief     当たり判定の形状を設定
    *****************************************************************************************/
    void SetTag(ColliderTag tag) { m_Tag = tag; }


    /*****************************************************************************************
    * @brief    当たり判定の主のタグを取得
    * @return   当たり判定の主のタグ
    *****************************************************************************************/
    ColliderTag GetTag() const { return m_Tag; }


    /*****************************************************************************************
    * @brief     存在しているかを設定
    *****************************************************************************************/
    void SetActive(bool flag) { m_IsActive = flag; }


    /*****************************************************************************************
    * @brief    当たり判定の主のタグを取得
    * @return   当たり判定の主のタグ
    *****************************************************************************************/
    bool GetActive() const { return m_IsActive; }


    /*****************************************************************************************
    * @brief     判定結果の一時的なフラグ (Strategyが設定)
    *****************************************************************************************/
    void SetHit(bool isHit) { m_IsHit = isHit; }


    /*****************************************************************************************
* @brief    当たり判定の主のタグを取得
* @return   当たり判定の主のタグ
*****************************************************************************************/

    bool GetHit() const { return m_IsHit; }

    const D3DXVECTOR3& GetWorldPosition() const { return m_WorldPosition; }

    // CollisionSphereのような子クラスからアクセスするためのヘルパー (必要な場合)
    const D3DXVECTOR3& GetLocalOffset() const { return m_LocalOffset; }

    /**
     * 親GameObjectの位置を参照し、ワールド座標を更新します。
     * CollisionManager::Update()から呼び出され、親の位置に追従します。
     */
    void UpdateWorldPosition();

    /**
     * 衝突イベントをリスナーに通知します。
     * CollisionManagerから衝突判定後に呼び出されます。
     * @param pOther 衝突相手のCollisionBaseオブジェクト
     */
    void InvokeCollision(CollisionBase* pOther);


protected:

    //オブジェクトのポジションを参照して同期
    const D3DXVECTOR3& m_SyncPosition;

    // 形状の相対位置 (親オブジェクトからの微調整オフセット)
    D3DXVECTOR3	m_LocalOffset;

    //現在の座標
    D3DXVECTOR3	m_WorldPosition;

    // コリジョンイベントの通知先
    ICollisionListener* m_pListener;

    // コリジョンの種類を識別するタグ
    ColliderTag m_Tag;

    // 判定のフラグ
    bool m_IsActive;

    // 直前の判定でヒットしたかどうかのフラグ
    bool m_IsHit;

private:
};