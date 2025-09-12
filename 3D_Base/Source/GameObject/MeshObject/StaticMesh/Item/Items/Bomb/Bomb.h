#pragma once
#include "Item/ItemBase.h"

class Bomb :
    public ItemBase
{
public:
    Bomb();
    ~Bomb();

    void Init()override;

    void Update(std::vector<std::unique_ptr<CPlayer>>& playiers) override;

    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)override;

    void Spawn()	override; // 出現
    void OnGround() override; // 取得待機
    void Have(std::vector<std::unique_ptr<CPlayer>>& playiers)	override; // 取得
    void Use(std::vector<std::unique_ptr<CPlayer>>& playiers)	override; // 使用
    void Throw()	override; // 投擲
    void Destroy()	override; // 消滅


};

