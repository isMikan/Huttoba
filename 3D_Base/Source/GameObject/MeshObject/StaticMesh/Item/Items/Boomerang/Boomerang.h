#pragma once
#include "Item/ItemBase.h"

class Boomerang
    : public ItemBase
{
public:

    enum class Effect
    {
        ChargeMax = 0,
        HitPlayer,
    };

    Boomerang();
    ~Boomerang();

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

    bool GetIsUseThrow() const { return m_IsUseThrow; }
    bool GetIsComeBack() const { return m_ComeBack; }

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

    //吹き飛ばし処理
    void Smash(CPlayerBase& playiers);

    //チャージ処理
    void PowerCharge();
    void FullCharge();

    //プレイヤーの行列に合わせる
    void TrackingPlayer();

    //プレイヤーに戻ってくる
    void ComeBackToPlayer();

private:
    //移動速度
    D3DXVECTOR3 m_Velocity;
    D3DXVECTOR3 m_AddVelocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //使用時の処理
    bool        m_IsUseThrow;

    bool        m_ComeBack;
    bool        m_IsCharge;
    bool        m_IsMaxCharge;

    std::unordered_map<Effect, ::EsHandle> m_hEffect;

    //連続ヒット防止のためにプレイヤーを保存する変数
    std::vector<CPlayerBase*> m_HitPlayer;
};