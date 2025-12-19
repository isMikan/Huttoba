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

    //状態変化時に一度だけ処理
    void ItemState(IItemObserver::State state)override;

private:
    //当たった際の処理
    void OnCollision(CollisionBase* other)override;

    //所持の移動処理
    void HaveMove();
    //使用の移動処理
    void UseMove();
    //投擲の移動処理
    void ThrowMove();

    //状態が使用時に変化したときの一度だけの処理
    //使用に状態が変化したときの処理
    void OneEnterUse();
    //投擲に状態が変化したときの処理
    void OneEnterThrow();

    //使用と投擲に状態が変化したときの共通処理
    void EnterUseThrowCommon();

    //爆発処理
    void Explosion();

    //吹き飛ばし処理
    void Smash(CPlayerBase& playiers);

    //爆弾の爆発前の色の変更処理
    void ChangeColor();

    //爆弾とプレイヤーの距離に応じて吹き飛ばし力を計算
    float CalculateForceScalar(float distance);

    //爆発の当たり判定メッシュの半径をとる
    bool LoadExplosionMesh();

private:
    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //投げた時のy軸の上昇量
    float       m_UpSpeed;

    //投げてから爆発するまでの時間
    float       m_ExplosionTime;

    //爆発するまでのカウント
    float       m_ExplosionCnt;

    //点滅の経過時間を記録
    float      m_ColorTimer;

    //爆発フラグ
    bool        m_IsExploded;

    //爆発の中心位置に近いほど多く吹き飛ぶので最小と最大の吹き飛ばし力を追加

    //爆発の最小吹き飛ばし力
    float       m_MinSmashPower;

    //爆発の最大吹き飛ばし力
    float       m_MaxSmashPower;

    //爆発の当たり判定のメッシュ半径を記録
    float       m_ExplosionRadius;
};