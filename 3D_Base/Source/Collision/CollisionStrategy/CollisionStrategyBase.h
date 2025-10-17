#pragma once
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"

//=====================================
// CollisionStrategyBase
// 
// 衝突判定アルゴリズムの基底クラス。
// 具体的な形状同士の判定（Sphere-Sphere 等）は
// このクラスを継承して実装する。
//=====================================
class CollisionStrategyBase
{
public:
	CollisionStrategyBase() = default;
	virtual ~CollisionStrategyBase() = default;

	// 衝突判定関数
	virtual CollisionResult CheckCollision(
		const CollisionBase* colA,
		const CollisionBase* colB) const = 0;

	// 衝突したかを返す関数
	bool IsCollide(const CollisionBase* colA, const CollisionBase* colB) const
	{
		if (!colA || !colB) return false;
		return CheckCollision(colA, colB).IsHit;
	}
};