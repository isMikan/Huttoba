#pragma once
#include "Collision/Collider/CollisionBase.h"

struct BoundingCapsule
{
	D3DXVECTOR3 StartPoint;		// 始点(ワールド)
	D3DXVECTOR3 EndPoint;		// 終点(ワールド)
	float Rad;	//半径
};

class CollisionCapsule : 
	public CollisionBase
{
public:
	// コンストラクタ: すべての初期情報を一括で受け取る
	CollisionCapsule(
		ICollisionListener* pListener,
		const D3DXVECTOR3 syncPosition,
		ColliderTag tag,
		float radius,
		const D3DXVECTOR3 localOffsetA, // 軸線分 A のローカルオフセット
		const D3DXVECTOR3 localOffsetB, // 軸線分 B のローカルオフセット
		const D3DXVECTOR3 localOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f) // Base用オフセット
	);	
	
	virtual ~CollisionCapsule() = default;

	virtual ColliderType GetType() const override { return ColliderType::Capsule; }

	void UpdateWorldPosition() override;

	const BoundingCapsule& GetWorldCapsule() const { return m_WorldCapsule; }
	void SetLocalOffSet(const D3DXVECTOR3& localOffsetA, const D3DXVECTOR3& localOffsetB);

	//カプセルを回転させる関数
	void SetLocalCapsuleRotation(const D3DXMATRIX& rotation) {
		m_LocalCapsuleRotation = rotation;
	}

	// 描画用関数
	const D3DXQUATERNION& GetDrawRotation() const { return m_DrawRotation; }
	float GetDrawLength() const { return m_DrawLength; }
	const D3DXVECTOR3& GetDrawCenterPosition() const { return m_DrawCenterPosition; }

private:

	// カプセル固有のローカルデータ
	FLOAT m_Radius;
	D3DXVECTOR3 m_LocalOffsetA;
	D3DXVECTOR3 m_LocalOffsetB;

	D3DXMATRIX m_LocalCapsuleRotation; // カプセル判定にのみ適用される回転
	// 最新のワールドカプセル情報
	BoundingCapsule m_WorldCapsule;

	// 描画専用データ
	D3DXQUATERNION m_DrawRotation;
	float m_DrawLength;
	D3DXVECTOR3 m_DrawCenterPosition;
};
