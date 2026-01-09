#pragma once
#include "Item/ItemBase.h"

class Mushroom 
    : public ItemBase
{
public:
    Mushroom();
    ~Mushroom();

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

    bool GetIsPlaced() const { return m_IsPlaced; }

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

    //吹き飛ばし処理
    void Smash(CPlayerBase& playiers);
    
    //スカラーの計算
    float CalculateForceScalar(float distance) const;

    //反射方向の計算
    //第一引数に入射ベクトル、第二引数に法線ベクトルをいれる
    //戻り値は正規化した方向ベクトルを返す
    D3DXVECTOR3 CalculateReflectionDirection(const D3DXVECTOR3& vIncomingDir, const D3DXVECTOR3& vNormal);


private:
    //使用可能時間
    float       m_UseDuration;

    //アイテムを設置したかをとる
    bool        m_IsPlaced;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時のy軸の上昇量
    float       m_UpSpeed;

    //投げている状態
    bool        m_IsThrow;

    //最小吹き飛ばし力
    float       m_MinSmashPower;

    //最大吹き飛ばし力
    float       m_MaxSmashPower;

    //きのこの大きさ変更で使用
    float       m_timer;

    ::EsHandle  m_hEffect;

    //投擲時の連続ヒット防止
    std::vector<CPlayerBase*> m_ThrowHitPlayer;

    //使用時の連続ヒット防止
    std::unordered_map<CPlayerBase*, float> m_UseHitPlayer;

    //使用時の連続ヒット防止用クールタイム
    float       m_UseHitCoolTime;
};