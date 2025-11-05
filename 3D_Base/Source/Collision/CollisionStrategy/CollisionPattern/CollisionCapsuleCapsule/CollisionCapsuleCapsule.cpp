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

	float s_nom = 0.0f; // s の分子
	float t_nom = 0.0f; // t の分子
	float s = 0.0f;     // 線分 A の媒介変数
	float t = 0.0f;     // 線分 B の媒介変数

	// 媒介変数の解決とクランプ (最短距離を決定するコアロジック)
	if (denom < EPSILON) // 線分がほぼ平行な場合
	{
		// 最短距離は線分の端点にあり、tは0にクランプされると仮定
		s = 0.0f;
		// t は線分 B 上で最も近い点を探す
		t_nom = e;
		t = t_nom / c;
		t = std::max(0.0f, std::min(1.0f, t));
	}
	else
	{
		// s = (b*e - c*d) / denom
		s_nom = (b * e - c * d);
		// t = (a*e - b*d) / denom
		t_nom = (a * e - b * d);

		// s, t を [0, 1] にクランプする複雑な処理

		// tの初期クランプ (0 <= s <= 1の条件付き)
		if (s_nom < 0.0f) // s < 0 の場合
		{
			s = 0.0f;
			t = std::max(0.0f, std::min(1.0f, e / c));
		}
		else if (s_nom > denom) // s > 1 の場合
		{
			s = 1.0f;
			t = std::max(0.0f, std::min(1.0f, (e + b) / c));
		}
		else // 0 <= s <= 1 の場合
		{
			s = s_nom / denom;
			t = std::max(0.0f, std::min(1.0f, t_nom / denom));
		}
	}
	// Note: この実装は s を先に確定させて t をクランプしている。
	// より厳密には t も同時にクランプ範囲を確認する必要があるらしい
	// でも結構この実装でいいらしい


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

	std::cout << (test == true ? "true": "false") << std::endl;

	return distanceSq <= totalRadiusSq;
}
