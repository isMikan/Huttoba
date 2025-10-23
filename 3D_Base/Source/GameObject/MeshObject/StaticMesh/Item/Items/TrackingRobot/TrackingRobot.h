#pragma once
#include "Item/ItemBase.h"

class TrackingRobot 
    : public ItemBase
{
public:
    TrackingRobot();
    ~TrackingRobot();

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

private:
    //アイテムを取得したときかを判定
    bool        m_IsTake;
    //アイテムを手に持つまでの時間
    float       m_PickUpTime;
    //アイテムを手に持つまでのカウント
    float       m_PickUpCnt;
};