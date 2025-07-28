#include "CPlayerRotation.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "CPlayerRotationIdle.h"

CPlayerRotation::CPlayerRotation()
	: m_RotationSpeed	( static_cast<FLOAT>(D3DXToRadian(5.0)) )
	, m_RotationDir		()
{
}

CPlayerRotation::~CPlayerRotation()
{
}

void CPlayerRotation::Enter(CPlayer& pPlayer)
{
}

void CPlayerRotation::Eixt(CPlayer& pPlayer)
{
}

void CPlayerRotation::Update(CPlayer& pPlayer)
{
	D3DXVECTOR3 rot = pPlayer.GetRotation();

	float diff = m_RotationDir - rot.y;	//目的方向までの差.

	//180度以上の位置は反時計回りのほうが早い.
	while (diff > D3DXToRadian(180.f))
	{
		diff -= D3DXToRadian(360.f);	//一周分引いてマイナスにする.
	}
	//180度以内の位置は時計回りのほうが早い.
	while (diff < -D3DXToRadian(180.f))
	{
		diff += D3DXToRadian(360.f);	//一周分足してプラスにする.
	}
	
	//回転量より小さかったら合わせる.
	if (abs(diff) < m_RotationSpeed)
	{
		rot.y = m_RotationDir;
	}
	//移動量を計算する.
	else if(diff > 0)
	{
		rot.y += m_RotationSpeed;
	}
	else
	{
		rot.y -= m_RotationSpeed;
	}

	pPlayer.SetRotation(rot);
}

void CPlayerRotation::Handle(CPlayer& pPlayer, int inputKey)
{
}

void CPlayerRotation::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x == 0 && z == 0)
	{
		pPlayer.SetRotationState(new CPlayerRotationIdle);
		return;
	}

	m_RotationDir = atan2(x, z);
}

std::string CPlayerRotation::GetStateName() const
{
	return "Rotation";
}
