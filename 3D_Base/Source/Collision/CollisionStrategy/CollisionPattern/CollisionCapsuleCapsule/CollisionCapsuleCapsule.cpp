#include "stdafx.h"
#include "CollisionCapsuleCapsule.h"

const auto EPSILON = 1e-5f;

bool CollisionCapsuleCapsule::CheckCollision(
    const CollisionBase* colA,
    const CollisionBase* colB) const
{

	// 各コライダーを適切な型にダウンキャスト
	const CollisionCapsule* capsuleA = dynamic_cast<const CollisionCapsule*>(colA);
	const CollisionCapsule* capsuleB = dynamic_cast<const CollisionCapsule*>(colB);

	if (!capsuleA || !capsuleB) { return false; }

	// ワールド座標のカプセル情報を取得
	const auto& worldCapsuleA = capsuleA->GetWorldCapsule();
	const auto& worldCapsuleB = capsuleB->GetWorldCapsule();

	// 線分Aの端点
	const D3DXVECTOR3& A1 = worldCapsuleA.StartPoint;
	const D3DXVECTOR3& A2 = worldCapsuleA.EndPoint;
	float R_A = worldCapsuleA.Rad;

	// 線分Bの端点
	const D3DXVECTOR3& B1 = worldCapsuleB.StartPoint;
	const D3DXVECTOR3& B2 = worldCapsuleB.EndPoint;
	float R_B = worldCapsuleB.Rad;

	// 線分間の最短距離を求めるためのパラメータ設定

	// 線分ベクトル
	D3DXVECTOR3 u = A2 - A1; // u = A2 - A1
	D3DXVECTOR3 v = B2 - B1; // v = B2 - B1
	D3DXVECTOR3 w = A1 - B1; // w = A1 - B1

	// 係数の計算
	float a = D3DXVec3LengthSq(&u); // u・u
	float b = D3DXVec3Dot(&u, &v);  // u・v
	float c = D3DXVec3LengthSq(&v); // v・v
	float d = D3DXVec3Dot(&u, &w);  // u・w
	float e = D3DXVec3Dot(&v, &w);  // v・w

	// 行列式の計算
	float denom = a * c - b * b;

	float s = 0.0f; // 線分 A の媒介変数
	float t = 0.0f; // 線分 B の媒介変数

	// 最短距離を計算するためのコアロジック
	if (denom < EPSILON) // 軸がほぼ平行な場合 (分母がゼロに近い)
	{
		// 最短距離は線分 B の端点にあり、t は [0, 1] にクランプされると仮定
		s = 0.0f; // s=0 に固定し、線分B上の最も近い点 t を探す
		if (a < EPSILON) // 線分 A が点である場合 (a = u・u がゼロに近い)
		{
			s = 0.0f;
			t = 0.0f; // A1 と B1 の距離を計算することになる
		}
		else // 線分 A は線分
		{
			t = D3DXVec3Dot(&v, &w) / c; // t = (v・w) / (v・v)
			t = std::max(0.0f, std::min(1.0f, t));
		}
	}
	else // 線分が交差または斜めの場合
	{
		//  s, t を線分の延長線上で計算
		float invDenom = 1.0f / denom;
		s = (b * D3DXVec3Dot(&v, &w) - c * D3DXVec3Dot(&u, &w)) * invDenom;
		t = (a * D3DXVec3Dot(&v, &w) - b * D3DXVec3Dot(&u, &w)) * invDenom;

		//  s を [0, 1] にクランプ
		if (s < 0.0f) s = 0.0f;
		else if (s > 1.0f) s = 1.0f;

		// s をクランプした状態で、t を線分 B 上の正しい点に再計算し、t もクランプ
		// s=0 の場合、t は線分 B 上で A1 に最も近い点になる
		if (s < EPSILON)
		{
			t = std::max(0.0f, std::min(1.0f, D3DXVec3Dot(&v, &w) / c));
		}
		// s=1 の場合、t は線分 B 上で A2 に最も近い点になる
		else if (s > 1.0f - EPSILON)
		{
			t = std::max(0.0f, std::min(1.0f, (D3DXVec3Dot(&v, &w) + b) / c));
		}
	}
	// Note: t も先にクランプしてから s を再計算するパターンもありますが、
	// この s を優先し t を再計算するパターンは、多くの衝突ライブラリで採用される標準的な方法の一つです。

	// 最短距離を計算

	// 線分Aの最短点 P(s)
	D3DXVECTOR3 P = A1 + u * s;

	// 線分Bの最短点 Q(t)
	D3DXVECTOR3 Q = B1 + v * t;

	// 最短距離のベクトル
	D3DXVECTOR3 diff = P - Q;

	// 最短距離の二乗
	float distanceSq = D3DXVec3LengthSq(&diff);

	// 衝突判定
	// 最短距離 <= (半径A + 半径B)
	float totalRadius = R_A + R_B;
	float totalRadiusSq = totalRadius * totalRadius;

	bool test = distanceSq <= totalRadiusSq;

	return distanceSq <= totalRadiusSq;
}
