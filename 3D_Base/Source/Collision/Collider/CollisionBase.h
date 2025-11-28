#pragma once

#include "Collision/CollisionListener/ICollisionListener.h"

/*********************************************
 * 衝突情報の基底クラス。
 *****/
class CollisionBase
{
public:

    //オブジェクトタグ
    enum class ColliderTag
    {
        Player = 0,
        HandAttack,
        Item, 
        Ground, 
        Haetataki, 
        SmashBat, 
        Fan,
        Mushroom, 
        Magnet,
        Boomerang,
        TrackingRobot,
        Bomb,

        TRobotSeachCol,

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

    // コンストラクタ
    CollisionBase(
        ICollisionListener* pListener,
        const D3DXVECTOR3  syncPosition,
        ColliderTag         tag,
        const D3DXVECTOR3&  localOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f)
    );

    //デストラクタ
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
    *****************************************************************************************/
    ICollisionListener* GetListener() const { return m_pListener; }


    /*****************************************************************************************
    * @brief     当たり判定の形状を設定 
    *****************************************************************************************/
    void SetTag(ColliderTag tag) { m_Tag = tag; }


    /*****************************************************************************************
    * @brief    当たり判定の主のタグを取得
    *****************************************************************************************/
    ColliderTag GetTag() const { return m_Tag; }


    /*****************************************************************************************
    * @brief     存在しているかを設定
    *****************************************************************************************/
    void SetActive(bool flag) { m_IsActive = flag; }


    /*****************************************************************************************
    * @brief    当たり判定の主のタグを取得
    *****************************************************************************************/
    bool GetActive() const { return m_IsActive; }


    /*****************************************************************************************
    * @brief     判定結果の一時的なフラグ (Strategyが設定)
    *****************************************************************************************/
    void SetHit(bool isHit) { m_IsHit = isHit; }


    /*****************************************************************************************
    * @brief    衝突しているか取得
    *****************************************************************************************/
    bool GetHit() const { return m_IsHit; }


    /*****************************************************************************************
    * @brief    当たり判定の主のタグを取得
    *****************************************************************************************/
    const D3DXVECTOR3& GetWorldPosition() const { return m_WorldPosition; }


    /*****************************************************************************************
    * @brief    オフセットを取得
    * 
    * @note     CollisionSphereのような子クラスからアクセスするためのヘルパー 
    *           現状使用していない。一応の設計
    *****************************************************************************************/
    const D3DXVECTOR3& GetLocalOffset() const { return m_LocalOffset; }
    void SetLocalOffset(D3DXVECTOR3& offset)  { m_LocalOffset = offset; }

    //カプセルのオフセットを設定
    void SetLocalOffSetToCapsule(const D3DXVECTOR3& localOffsetA, const D3DXVECTOR3& localOffsetB);

    //カプセルの回転を設定
    void RotationXCapsule(float rad);


    /*****************************************************************************************
    * @brief    親GameObjectの位置を参照し、ワールド座標を更新する
    * 
    * @note     CollisionManager::Update()から呼び出す
    *****************************************************************************************/
    virtual void UpdateWorldPosition() = 0;


    /*****************************************************************************************
    * @brief    衝突イベントをリスナーに通知する
    * @param    pOther 衝突相手のCollisionBaseオブジェクト
    * 
    * @note     CollisionManagerから衝突判定後に呼び出される。
    *****************************************************************************************/
    void InvokeCollision(CollisionBase* pOther);


protected:

    //オブジェクトのポジションを参照して同期
    const D3DXVECTOR3& m_SyncPosition;

    // 形状の相対位置 (親オブジェクトからの微調整オフセット)
    D3DXVECTOR3	m_LocalOffset;

    //カプセルの移動用オフセット
    D3DXVECTOR3 m_MoveOffsetA;
    D3DXVECTOR3 m_MoveOffsetB;

    //カプセルのローカルオフセット
    D3DXVECTOR3 m_LocalOffsetA;
    D3DXVECTOR3 m_LocalOffsetB;

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
};