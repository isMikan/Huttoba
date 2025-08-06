#include "CPlayerRotation.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerRotationState/PlayerRotationIdle/CPlayerRotationIdle.h"

CPlayerRotation::CPlayerRotation()
	: m_RotationSpeed	( D3DXToRadian( 5.f ) )
	, m_RotationDir		()
{
}

CPlayerRotation::~CPlayerRotation()
{
}

void CPlayerRotation::Enter(CPlayer& pPlayer)
{
}

void CPlayerRotation::Exit(CPlayer& pPlayer)
{
}

void CPlayerRotation::Update(CPlayer& pPlayer)
{
	//回転しているかを true に設定.
	pPlayer.SetRotating(true);

	//回転を取得.
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
		//回転が目的のところまで到達しているので false に設定.
		pPlayer.SetRotating(false);
	}
	//移動量を計算する.
	else if (diff > 0)
	{
		//正の数なら足す.
		rot.y += m_RotationSpeed;
	}
	else
	{
		//負の数なら引く.
		rot.y -= m_RotationSpeed;
	}

	//方向を設定.
	pPlayer.SetRotation(rot);
}

void CPlayerRotation::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x == 0 && z == 0)
	{
		pPlayer.SetRotationState(std::make_unique<CPlayerRotationIdle>());
		return;
	}

	m_RotationDir = atan2(x, z);
}

std::string CPlayerRotation::GetStateName() const
{
	return "Rotation";
}
