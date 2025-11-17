#pragma once
#include "Item/ItemBase.h"

class CPlayer;

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
    void ChangeState(State state)override;

private:
    //当たった際の処理
    void OnCollision(CollisionBase* other)override;

    //所持中の移動処理
    void HaveMove();
    //使用中の移動処理
    void UseMove();
    //投擲の移動処理
    void ThrowMove();

    //爆発処理
    void Explosion();

    //吹き飛ばし処理
    void Smash(CPlayer& playiers);

    //爆発前の色の変更処理
    void ChangeColor();

    //プレイヤーの距離に応じて吹き飛ばし力を計算
    float CalculateForceScalar(float distance);

    //追尾処理
    void Homing(D3DXVECTOR3 targetPos);

    //前方索敵用
    void SearchForward();

    //一番近いプレイヤーを判定
    void FindNearestTarget();

private:

    std::vector<CPlayerBase*> m_pTargetList;

    CPlayerBase* m_pTarget;

    bool m_IsHoming;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //投げる時に一度だけ処理するものがあるので追加
    bool        m_IsThrow;

    //投げてから爆発するまでの時間
    double       m_ExplosionTime;

    //爆発するまでのカウント
    double       m_ExplosionCnt;

    //爆発の吹き飛ばしの力
    float       m_KnockBackPower;

    //点滅の経過時間を記録
    double      m_ColorTimer;

    //爆発時の処理
    bool        m_IsExploded;

    //索敵の当たり判定の位置の補正値
    D3DXVECTOR3 m_CollisionOffSet;
};