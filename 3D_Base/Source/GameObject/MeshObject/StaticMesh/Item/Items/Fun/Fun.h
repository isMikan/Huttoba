#pragma once
#include "Item/ItemBase.h"

class Fun
    : public ItemBase
{
public:
    Fun();
    ~Fun();

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

    //当たった際の処理
    void Hit(CPlayerBase& playiers);

private:
    //アイテムを使用中かどうかを判定
    bool        m_IsUse;

    //アイテムを持った際の補正値
    D3DXVECTOR3 m_HaveOffset;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //投げる時に一度だけ処理するものがあるので追加
    bool        m_IsThrow;

    //当たり判定を切り替えるために追加
    std::shared_ptr<CollisionBase>	m_pNowCollider;	

    //手に持っている状態
    bool        m_IsHold;
};