#pragma once
#include "Item/ItemBase.h"

class Bomb
    : public ItemBase
{
public:
    Bomb();
    ~Bomb();

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
    void Smash(CPlayerBase& playiers);

    //爆弾の爆発前の色の変更処理
    void ChangeColor();

    //爆弾とプレイヤーの距離に応じて吹き飛ばし力を計算
    float CalculateForceScalar(float distance);

private:
    //アイテムを取得したときかを判定
    bool        m_IsTake;
    //アイテムを手に持つまでの時間
    float       m_PickUpTime;
    //アイテムを手に持つまでのカウント
    double      m_PickUpCnt;

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

    //点滅の経過時間を記録
    double      m_ColorTimer;

    //爆発時の処理
    bool        m_IsExploded;

    //爆発の中心位置に近いほど多く吹き飛ぶので最小と最大の吹き飛ばし力を追加

    //爆発の最小吹き飛ばし力
    float       m_MinSmashPower;

    //爆発の最大吹き飛ばし力
    float       m_MaxSmashPower;

    //手に持っている状態
    bool        m_IsHold;
};