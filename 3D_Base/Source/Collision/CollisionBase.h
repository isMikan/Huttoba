#pragma once

#include<string>
#include<memory>

#include "GameObject/CGameObject.h"

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

		Unknown,
	};

	//ç¿ïWê›íËä÷êî.
	void SetPosition(float x, float y, float z) {
		m_vPosition.x = x;
		m_vPosition.y = y;
		m_vPosition.z = z;
	};
	void SetPosition(const D3DXVECTOR3& pos) {
		m_vPosition = pos;
	}

	//âÒì]ê›íËä÷êî.
	void SetRotation(float x, float y, float z) {
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	};
	void SetRotation(const D3DXVECTOR3& rot) {
		m_vRotation = rot;
	}

	//ägèkê›íËä÷êî.
	void SetScale(float x, float y, float z) {
		m_vScale.x = x;
		m_vScale.y = y;
		m_vScale.z = z;
	}
	void SetScale(float xyz) {
		m_vScale = D3DXVECTOR3(xyz, xyz, xyz);
	}

	//éÊìæä÷êî.
	const D3DXVECTOR3& GetPosition() const {return m_vPosition;}
	const D3DXVECTOR3& GetRotation() const {return m_vRotation;}
	const D3DXVECTOR3& GetScale() const {return m_vScale;}

	//--------------------------------------------------------------------------------------------------------------

	virtual void UpdateWorldMat() = 0;
	virtual const D3DXMATRIX& GetWorldMat() const = 0;
	virtual D3DXVECTOR3 GetWorldPos() const = 0;

	void SetOwner(const CGameObject* obj) { Owner = obj; }
	const CGameObject* GetOwner() const { return Owner; }

	void SetTag(ColliderTag tag) { Tag = tag; }
	ColliderTag GetTag() const { return Tag; }

	void SetActive(bool flag) { IsActive = flag; }
	bool GetActive() const { return IsActive; }


public:

	//ÉgÉâÉìÉXÉtÉHÅ[ÉÄån
	D3DXVECTOR3	m_vPosition;
	D3DXVECTOR3	m_vRotation;
	D3DXVECTOR3	m_vScale;

protected:

	const CGameObject* Owner;
	ColliderTag Tag = ColliderTag::Unknown;
	bool IsActive;

private:

};