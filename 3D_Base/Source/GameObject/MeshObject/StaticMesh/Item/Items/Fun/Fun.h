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

    //投擲に状態が変化したときの処理
    void OneEnterThrow();

    //当たった際の処理
    void Hit(CPlayerBase& playiers);

private:
    //使用可能時間
    float       m_UseDuration;

    //アイテムを持った際の補正値
    D3DXVECTOR3 m_HaveOffset;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //押し出しの強さ
    float       m_Power;

    //当たり判定を切り替えるために追加
    std::shared_ptr<CollisionBase>	m_pNowCollider;	

    ::EsHandle hEffect = 1;

    std::shared_ptr<CollisionBase>	m_pUseCollider_2;		//使用中用当たり判定
    std::vector<CPlayerBase*> m_ThrowHitPlayer;

};