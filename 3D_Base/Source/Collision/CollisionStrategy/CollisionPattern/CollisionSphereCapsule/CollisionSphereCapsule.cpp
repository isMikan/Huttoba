#include "stdafx.h"
#include "CollisionSphereCapsule.h"

bool CollisionSphereCapsule::CheckCollision(
    const CollisionBase* colA,
    const CollisionBase* colB) const
{
	// 1. 各コライダーを適切な型にダウンキャスト
	const CollisionCapsule* capsule = nullptr;
	const CollisionSphere* sphere = nullptr;

	// ColA, ColBのどちらがCapsuleでSphereかわからないため、両方試す
	if (colA->GetType() == CollisionBase::ColliderType::Capsule && 
		colB->GetType() == CollisionBase::ColliderType::Sphere)
	{
		capsule = dynamic_cast<const CollisionCapsule*>(colA);
		sphere = dynamic_cast<const CollisionSphere*>(colB);
	}
	else if (colA->GetType() == CollisionBase::ColliderType::Sphere &&
		colB->GetType() == CollisionBase::ColliderType::Capsule)
	{
		sphere = dynamic_cast<const CollisionSphere*>(colA);
		capsule = dynamic_cast<const CollisionCapsule*>(colB);
	}

	// 念のためのガード
	if (!capsule || !sphere){ return false; }

	// ワールド座標のカプセル情報と球情報を取得
	const auto& worldCapsule = capsule->GetWorldCapsule();
	const D3DXVECTOR3& centerS = sphere->GetWorldPosition();
	float radiusS = sphere->GetRadius();

	const D3DXVECTOR3& A = worldCapsule.StartPoint;
	const D3DXVECTOR3& B = worldCapsule.EndPoint;
	float radiusC = worldCapsule.Rad;

	// 2. カプセルの軸線分 (A-B) 上の、球の中心 (centerS) に最も近い点 (P) を求める

	// 線分ベクトル (B - A)
	D3DXVECTOR3 AB = B - A;

	// 球の中心からAへのベクトル (centerS - A)
	D3DXVECTOR3 AS = centerS - A;

	// AB上へのASの射影 (t) を計算
	// t = dot(AS, AB) / dot(AB, AB)
	float lengthSqAB = D3DXVec3LengthSq(&AB);
	float t = 0.0f;

	if (lengthSqAB > 0.0f)
	{
		// ABがゼロベクトルでない場合
		t = D3DXVec3Dot(&AS, &AB) / lengthSqAB;

		// tを0.0～1.0の範囲にクランプ（線分上に限定）
		t = std::max(0.0f, std::min(1.0f, t));
	}
	// else: lengthSqAB == 0.0 の場合、tは0のまま。AとBが同じ点で、球vs球判定になる。

	// 3. 最も近い点 P の座標を計算
	D3DXVECTOR3 P = A + (AB * t);

	// 4. 最も近い点 (P) と 球の中心 (centerS) の距離を計算
	D3DXVECTOR3 diff = centerS - P;
	float distanceSq = D3DXVec3LengthSq(&diff);

	// 5. 衝突判定
	// 最短距離 <= (カプセルの半径 + 球の半径)
	float totalRadius = radiusC + radiusS;
	float totalRadiusSq = totalRadius * totalRadius;

	return distanceSq <= totalRadiusSq;
}