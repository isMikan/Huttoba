#pragma once
#include <d3dx9.h>  // D3DXVECTOR3 を使うために必要

//--------------------------------------------------------------------------------------------------------------
// 判定結果構造体
// 
// ・判定結果を返すことを目的とした構造体
// 
// ─ 構成方針 ─
// Collider = 当たり判定の設定（位置・形状など）
// Manager  = 衝突判定の管理
// Strategy = 当たり判定のアルゴリズム選択
// Result   = 当たり判定の結果データ
// 
// と考え中。
 //--------------------------------------------------------------------------------------------------------------
struct CollisionResult
{
	bool IsHit;             // 衝突したか
	D3DXVECTOR3 HitPoint;   // 衝突位置
	D3DXVECTOR3 HitNormal;  // 衝突面の法線
	float Penetration;      // めり込み深さ

	CollisionResult()
		: IsHit(false)
		, HitPoint(0, 0, 0)
		, HitNormal(0, 1, 0)
		, Penetration(0.0f)
	{
	}
};
