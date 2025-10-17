#pragma once

#include<string>
#include<memory>

#include "GameObject/CGameObject.h"

class ICollisionListener;

class CollisionBase
{
public:
	CollisionBase();
	virtual ~CollisionBase();

	enum class ColliderTag
	{
		Player,
		Item,
		Ground,

		Haetataki,
		SmashBat,
		Fan,
		Mushroom,
		Magnet,
		Boomerang,
		RatRobot,

		None,
	};

	enum class CollisionType
	{
		Capsule,
		OBB,
		Ray,
		Sphere,

		None,
	};

	//座標設定関数.
	void SetPosition(float x, float y, float z) {
		m_vPosition.x = x;
		m_vPosition.y = y;
		m_vPosition.z = z;
	};
	void SetPosition(const D3DXVECTOR3& pos) {
		m_vPosition = pos;
	}

	//回転設定関数.
	void SetRotation(float x, float y, float z) {
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	};
	void SetRotation(const D3DXVECTOR3& rot) {
		m_vRotation = rot;
	}

	//拡縮設定関数.
	void SetScale(float x, float y, float z) {
		m_vScale.x = x;
		m_vScale.y = y;
		m_vScale.z = z;
	}
	void SetScale(float xyz) {
		m_vScale = D3DXVECTOR3(xyz, xyz, xyz);
	}

	//取得関数.
	const D3DXVECTOR3& GetPosition() const {return m_vPosition;}
	const D3DXVECTOR3& GetRotation() const {return m_vRotation;}
	const D3DXVECTOR3& GetScale() const {return m_vScale;}

	//--------------------------------------------------------------------------------------------------------------

	//ローカル座標をワールド座標に変換
	virtual void UpdateWorldMat() = 0;

	//ワールド行列取得
	virtual const D3DXMATRIX& GetWorldMat() const = 0;

	//現在座標の中心を取得
	virtual D3DXVECTOR3 GetWorldPos() const = 0;

	//当たり判定の主を設定
	void SetOwner(const CGameObject* obj) { m_Owner = obj; }
	const CGameObject* GetOwner() const { return m_Owner; }

	//主の種類を設定
	void SetTag(ColliderTag tag) { m_Tag = tag; }

	//存在しているか設定
	void SetActive(bool flag) { m_IsActive = flag; }


	ColliderTag GetTag() const { return m_Tag; }
	bool GetActive() const { return m_IsActive; }


public:

	//トランスフォーム系
	D3DXVECTOR3	m_vPosition;
	D3DXVECTOR3	m_vRotation;
	D3DXVECTOR3	m_vScale;

protected:

	const CGameObject* m_Owner;
	ColliderTag m_Tag;
	bool m_IsActive;
private:

};