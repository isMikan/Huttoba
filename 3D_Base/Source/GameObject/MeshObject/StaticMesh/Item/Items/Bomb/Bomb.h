#pragma once
#include "Item/ItemBase.h"

class Bomb
    : public ItemBase
{
public:
    Bomb();
    ~Bomb();

    void Init()override;

    void Update(std::unique_ptr<CPlayerManager>& playiers)override;

    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)override;

    //出現
    void Spawn()override;

    //取得待機
    void OnGround()override;
    //取得
    void Have(std::unique_ptr<CPlayerManager>& playiers)override;
    //使用
    void Use(std::unique_ptr<CPlayerManager>& playiers)override;
    //投擲
    void Throw(std::unique_ptr<CPlayerManager>& playiers)override;
    //消滅
    void Destroy()override;

private:
    //取得モーション
    void TakeMotion();
    //所持中モーション
    void PossessionMotion(std::unique_ptr<CPlayerManager>& playiers);
    //使用モーション
    void UseMotion();
    //投擲モーション
    void ThrowMotion();

    //使用と投擲が同じ処理なのでひとまとめにする
    void UseAndThrow(std::unique_ptr<CPlayerManager>& playiers);

    //爆発処理
    void Explosion(std::unique_ptr<CPlayerManager>& playiers);

    //吹き飛ばし処理(仮)
    void Blow_Away(std::unique_ptr<CPlayerManager>& playiers);

    //爆弾の爆発前の色の変更処理
    void ChangeColor();

    //爆弾とプレイヤーの距離に応じて吹き飛ばし力を計算
    //線形補間を使用
    float CalculateKnockBackPower(float distance);

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

    //爆発の吹き飛ばしの力
    float       m_KnockBackPower;

    //点滅の経過時間を記録
    double      m_ColorTimer;
};