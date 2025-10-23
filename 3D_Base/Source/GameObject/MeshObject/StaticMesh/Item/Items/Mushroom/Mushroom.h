#pragma once
#include "Item/ItemBase.h"

class Mushroom 
    : public ItemBase
{
public:
    Mushroom();
    ~Mushroom();

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

    //当たった際の処理
    void Hit(std::unique_ptr<CPlayerManager>& playiers);

    //キノコとプレイヤーの距離に応じて吹き飛ばし力を計算
    float CalculateKnockBackPower(float distance);

private:
    //アイテムを取得したときかを判定
    bool        m_IsTake;
    //アイテムを手に持つまでの時間
    float       m_PickUpTime;
    //アイテムを手に持つまでのカウント
    float       m_PickUpCnt;

    //アイテムを持った際の補正値
    D3DXVECTOR3 m_HaveOffset;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //投げる時に一度だけ処理するものがあるので追加
    bool        m_IsThrow;
};