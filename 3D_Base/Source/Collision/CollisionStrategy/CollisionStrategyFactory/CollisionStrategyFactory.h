#pragma once
#include <unordered_map>
#include <memory>
#include "../../Collider/CollisionBase.h"

class CollisionStrategyBase;

class CollisionStrategyFactory
{
public:

    //各判定クラスを入れるための型
    using StrategyPtr = std::unique_ptr<CollisionStrategyBase>;

    /// <summary>
    /// シングルトンインスタンスを取得
    /// </summary>
    /// 
    /// <returns>CollisionStrategyFactory クラスの唯一のインスタンスへのポインタ。</returns>
    static CollisionStrategyFactory* GetInstance()
    {
        static CollisionStrategyFactory instance;
        return &instance;
    }

    /// <summary>
    /// 2つのコライダータイプ間の衝突判定を登録。
    /// </summary>
    /// <param name="typeA">最初のコライダーのタイプ。</param>
    /// <param name="typeB">2番目のコライダーのタイプ。</param>
    /// <param name="strategy">登録する衝突戦略のポインタ。</param>
    void RegisterStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB,
        StrategyPtr strategy);

    /// <summary>
    /// 2つのコライダータイプに基づいて衝突判定を取得します。
    /// </summary>
    /// <param name="typeA">最初のコライダーのタイプ。</param>
    /// <param name="typeB">2番目のコライダーのタイプ。</param>
    /// <returns>指定されたコライダータイプに対応するCollisionStrategyBaseのポインタ。</returns>
    CollisionStrategyBase* GetStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB);

private:
    CollisionStrategyFactory() = default;
    ~CollisionStrategyFactory() = default;

    // ペアのハッシュ値を計算する関数オブジェクト。
    struct PairHash
    {
        size_t operator()(const std::pair<CollisionBase::ColliderType, CollisionBase::ColliderType>& p) const noexcept
        {
            size_t h1 = std::hash<int>()(static_cast<int>(p.first));
            size_t h2 = std::hash<int>()(static_cast<int>(p.second));

            // 並び替えて常に小さい方を先に
            if (h1 > h2) std::swap(h1, h2);

            // Boost式合成
            return h1 ^ (h2 << 1);
        }
    };

    /// 衝突タイプのペアをキーとして、対応する戦略ポインタを格納するためのマップ
    std::unordered_map<std::pair<CollisionBase::ColliderType, CollisionBase::ColliderType>,
        StrategyPtr, PairHash> m_Strategies;
};