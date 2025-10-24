#pragma once
#include <unordered_map>
#include <memory>
#include "../../Collider/CollisionBase.h"

class CollisionStrategyBase;

class CollisionStrategyFactory
{
public:

    //各クラスの衝突戦略を入れるための型
    using StrategyPtr = std::unique_ptr<CollisionStrategyBase>;


    /*****************************************************************************************
    * @brief    シングルトンインスタンスを取得
    *****************************************************************************************/
    static CollisionStrategyFactory* GetInstance()
    {
        static CollisionStrategyFactory instance;
        return &instance;
    }


    /*****************************************************************************************
    * @brief    2つのコライダータイプ間の衝突判定を登録
    * @param    typeA   : 1つ目のコライダーのタイプ
    *           typeB   : 2つ目のコライダーのタイプ
    *           strategy: 登録する衝突戦略のポインタ
    *****************************************************************************************/
    void RegisterStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB,
        StrategyPtr strategy);


    /*****************************************************************************************
    * @brief    2つのコライダータイプに基づいて衝突判定を取得
    * @param     typeA   : 1つ目のコライダーのタイプ
    *            typeB   : 2つ目のコライダーのタイプ
    *
    * @return   指定されたコライダータイプに対応するクラス(衝突戦略)のポインタ
    *****************************************************************************************/
    CollisionStrategyBase* GetStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB);

private:
    CollisionStrategyFactory() = default;
    ~CollisionStrategyFactory() = default;

    // mapに入れるためpairをハッシュ値に
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