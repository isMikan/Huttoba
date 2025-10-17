#pragma once
#include "Collision/CollisionBase.h"

class CollisionCapsule : 
	public CollisionBase
{
public:
	CollisionCapsule();
	~CollisionCapsule();

	//ローカル座標をワールドに変換
	void UpdateWorldMat() override;

	//ワールド行列を取得
	const D3DXMATRIX& GetWorldMat() const override { return m_World; }

	//現在座標の中心を取得
	D3DXVECTOR3 GetWorldPos() const override { return D3DXVECTOR3(m_World._41, m_World._42, m_World._43); }


	// ゲッター
	const D3DXVECTOR3& GetStart	() const { return m_Start; }
	const D3DXVECTOR3& GetEnd	() const { return m_End; }
	float GetRadius() const { return m_Radius; }

	// セッター
	void SetStart (const D3DXVECTOR3& s) { m_Start = s; }
	void SetEnd	  (const D3DXVECTOR3& e) { m_End = e; }
	void SetRadius(float r) { m_Radius = r; }


	// ワールド座標変換後の線分情報取得
	D3DXVECTOR3 GetStartWorld() const;
	D3DXVECTOR3 GetEndWorld() const;

	float GetWorldRadius() const;
private:
	D3DXVECTOR3 m_Start;  // ローカル開始点
	D3DXVECTOR3 m_End;    // ローカル終了点
	float m_Radius;       // 半径

	D3DXMATRIX m_World;   // ワールド行列

	//ワールド変換座標
	D3DXVECTOR3 m_StartWorld;
	D3DXVECTOR3	m_EndWorld;
		
};
