#pragma once

#include <d3dx9math.h> // D3DXVECTOR3, D3DXMATRIX を使用
#include <memory>
#include <string>

// 前方宣言: 衝突イベントの通知先となるオブジェクト
class ICollisionListener;

/**
 * コリジョンオブジェクトの基底クラス (データコンテナ層)。
 * * 役割: 衝突形状の種類、識別タグ、および親オブジェクトからの
 * 相対的な位置（オフセット）といった「情報」のみを保持する。
 * * 注意: ワールド座標、回転、拡縮といった「絶対トランスフォーム」は
 * 親となる CGameObject が保持し、Strategyで利用される。
 */
class CollisionBase
{
public:
    // ----------------------------------------------------
    // 識別子 (メタ情報)
    // ----------------------------------------------------

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

    enum class ColliderType
    {
        Capsule = 0,
        OBB, 
        Ray,
        Sphere,

        None = -1,
    };

    // コンストラクタ / デストラクタ
    CollisionBase();
    virtual ~CollisionBase() = default;

    // ----------------------------------------------------
    // トランスフォーム情報 (相対座標: オフセット)
    // ----------------------------------------------------
    // コリダーの中心が、親オブジェクトの中心からどれだけズレているかを示す相対座標。
    // WorldPosition = CGameObject::GetPosition() + GetLocalOffset() で計算される。
    void SetLocalOffset(float x, float y, float z) {
        m_vLocalOffset.x = x;
        m_vLocalOffset.y = y;
        m_vLocalOffset.z = z;
    };
    void SetLocalOffset(const D3DXVECTOR3& offset) {
        m_vLocalOffset = offset;
    }

    const D3DXVECTOR3& GetLocalOffset() const { return m_vLocalOffset; }

    // ----------------------------------------------------
    // 必須情報 / 状態
    // ----------------------------------------------------

    // コリダーがどの種類の形状であるかを返す (純粋仮想関数)
    virtual ColliderType GetType() const = 0;

    // 当たり判定の主を設定 (ICollisionListenerへ通知するため)
    void SetOwner(ICollisionListener* obj) { m_Owner = obj; }
    ICollisionListener* GetOwner() const { return m_Owner; }

    // 主の種類を設定 (フィルタリング用)
    void SetTag(ColliderTag tag) { m_Tag = tag; }
    ColliderTag GetTag() const { return m_Tag; }

    // 存在しているか設定 (判定の有効/無効を切り替える)
    void SetActive(bool flag) { m_IsActive = flag; }
    bool GetActive() const { return m_IsActive; }

    // 判定結果の一時的なフラグ (Strategyが設定し、システムがリセットする)
    void SetHit(bool isHit) { m_IsHit = isHit; }
    bool GetHit() const { return m_IsHit; }


protected:

    // 形状の相対位置 (親オブジェクトからの微調整オフセット)
    D3DXVECTOR3	m_vLocalOffset;

    // コリジョンイベントの通知先
    ICollisionListener* m_Owner;

    // コリジョンの種類を識別するタグ (例: プレイヤー、アイテム)
    ColliderTag m_Tag;

    // 判定の有効/無効フラグ
    bool m_IsActive;

    // 直前の判定でヒットしたかどうかのフラグ
    bool m_IsHit;

private:
};