#pragma once
#include "Item/ItemBase.h"

class Bomb :
    public ItemBase
{
public:
    Bomb();
    ~Bomb();

    void Init()override;

    void Update(std::vector<std::unique_ptr<CPlayerBase>>& playiers)override;

    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)override;

    //出現
    void Spawn()override;

    //取得待機
    void OnGround()override;
    //取得
    void Have(std::vector<std::unique_ptr<CPlayerBase>>& playiers)override; 
    //使用
    void Use(std::vector<std::unique_ptr<CPlayerBase>>& playiers)override; 
    //投擲
    void Throw()override;
    //消滅
    void Destroy()override;

private:
    //使用モーション
    void UseMotion();
    //投擲モーション
    void ThrowMotion();
    //取得モーション
    void TakeMotion();
};