#pragma once
#include "Item/ItemBase.h"

class CPlayer;
class ChaseSensor;

class TrackingRobot
    : public ItemBase
{
public:
    TrackingRobot();
    ~TrackingRobot();

    void Init()override;

    void Update()override;

    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)override;

    //出現
    void Spawn()override;

    //取得待機
    void OnGround()override;
    //取得
    void Have()override;
    //使用
    void Use()override;
    //投擲
    void Throw()override;
    //消滅
    void Destroy()override;

    //状態変化時に一度だけ処理
    void ItemState(IItemObserver::State state)override;

private:
    //当たった際の処理
    void OnCollision(CollisionBase* other)override;

    //所持中の移動処理
    void HaveMove();
    //使用中の移動処理
    void UseMove();
    //投擲の移動処理
    void ThrowMove();

    //状態が使用時に変化したときの一度だけの処理
    //使用に状態が変化したときの処理
    void OneEnterUse();
    //投擲に状態が変化したときの処理
    void OneEnterThrow();

    //爆発処理
    void Explosion();

    //吹き飛ばし処理
    void Smash(CPlayerBase& playiers);

    //プレイヤーの距離に応じて吹き飛ばし力を計算
    float CalculateForceScalar(float distance);

    //追尾処理
    void Homing(D3DXVECTOR3 targetPos);

    //一番近いプレイヤーをターゲットに設定
    void UpdateChaseSensor();

    //爆発の当たり判定メッシュの半径をとる
    bool LoadExplosionMesh();

private:
    CPlayerBase*                    m_pTarget;

    //索敵判定クラス
    std::unique_ptr<ChaseSensor>    m_pChaseSensor;

    //地面に当たっているか
    bool        m_IsGround;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //使用時の移動速度
    float       m_MoveSpeed;

    //旋回の角度
    float       m_TurnRate;

    //投げた時の移動速度
    float       m_ThrowSpeed;

    //投げてから爆発するまでの時間
    double      m_ExplosionTime;

    //爆発するまでのカウント
    double      m_ExplosionCnt;

    //爆発の吹き飛ばしの力
    float       m_KnockBackPower;

    //爆発時の処理
    bool        m_IsExploded;

    bool        m_Is;

    //爆発の当たり判定のメッシュ半径を記録
    float       m_ExplosionRadius;

    //索敵の当たり判定の位置の補正値
    D3DXVECTOR3 m_CollisionOffSet;

    //連続ヒット防止
    std::vector<CPlayerBase*> m_ThrowHitPlayer;

};