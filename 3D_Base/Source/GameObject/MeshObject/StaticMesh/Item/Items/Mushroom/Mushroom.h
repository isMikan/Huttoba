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

private:
    //当たった際の処理
    void OnCollision(CollisionBase* other)override;

    //所持中の移動処理
    void HaveMove();
    //使用中の移動処理
    void UseMove();
    //投擲の移動処理
    void ThrowMove();

    //吹き飛ばし処理
    void Smash(CPlayerBase& playiers);
    
    //スカラーの計算
    float CalculateForceScalar(float distance);

    //反射方向の計算
    //第一引数に入射ベクトル、第二引数に法線ベクトルをいれる
    //戻り値が正規化した方向ベクトルを返す
    D3DXVECTOR3 CalculateReflectionDirection(const D3DXVECTOR3& vIncomingDir, const D3DXVECTOR3& vNormal);

private:
    //アイテムを設置したかをとる
    bool        m_IsPlaced;

    //移動速度
    D3DXVECTOR3 m_Velocity;

    //投げた時の移動速度
    float       m_MoveSpeed;

    //投げている状態
    bool        m_IsThrow;

    //投げる時に一度だけ処理するものがあるので追加
    bool        m_IsHasThrow;

    //最小吹き飛ばし力
    float       m_MinSmashPower;

    //最大吹き飛ばし力
    float       m_MaxSmashPower;

    //手に持っている状態
    bool        m_IsHold;
};