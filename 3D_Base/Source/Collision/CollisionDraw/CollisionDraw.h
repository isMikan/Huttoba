#pragma once

#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "GameObject/CGameObject.h"

class CGameObject; // 前方宣言

// 追従すべき対象と、描画するメッシュをセットで管理
struct DebugCollisionDrawEntry
{
    // 主オブジェクトのポインタ。このオブジェクトの動きに追従する。
    const CGameObject* pOwner;

    // 描画するデバッグメッシュ
    std::shared_ptr<CStaticMesh> pMesh;

    //描画するコライダーデータ
    std::weak_ptr<const CollisionBase> pCollider;
};

class CollisionDraw
{
public:

    static CollisionDraw* GetInstance()
    {
        static CollisionDraw instance;
        return &instance;
    }
    // デストラクタはpublicのままにして、メモリ管理をシステムに任せる
    ~CollisionDraw() = default;

    // 描画する当たり判定とオーナーポインタ登録
    void AddDrawMesh(
        const std::shared_ptr<CStaticMesh> pMesh,
        const CGameObject* pOwner,
        std::weak_ptr<const CollisionBase> pCollider
    );

    // 描画情報の削除
    void RemoveByOwner(const CGameObject* pOwner);

    // 描画処理
    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);


private:
    // 外部からのインスタンス化を禁止（シングルトン制御）
    CollisionDraw();
    CollisionDraw(const CollisionDraw&) = delete;
    CollisionDraw& operator=(const CollisionDraw&) = delete;

private:
    std::vector<DebugCollisionDrawEntry> m_pCollisionEntries;
};

