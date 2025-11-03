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
};

class CollisionDraw
    : public CGameObject
{
public:

    // ★ 登録関数を修正：CGameObject* も引数として受け取る
    void AddDrawMesh(
        const std::shared_ptr<CStaticMesh> pMesh,
        const CGameObject* pOwner // ★追従対象の主オブジェクト
    );

    /*****************************************************************************************
    * @brief    衝突判定から除外/破棄された当たり判定の削除処理
    * @param    破棄したい当たり判定
    *****************************************************************************************/
    void RemoveCollider(std::shared_ptr<CStaticMesh>& pColliderToRemove);

    void Update() override;


	//描画処理
    void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;


private:
    CollisionDraw();
    ~CollisionDraw();

    CollisionDraw(const CollisionDraw&) = delete; // コピーコンストラクタを削除
    CollisionDraw& operator=(const CollisionDraw&) = delete; // 代入演算子を削除

    std::vector<DebugCollisionDrawEntry> m_pCollisionEntries;
};

