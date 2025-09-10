#include "CPlayerRotation.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/DirectionalInputState/PlayerRotationState/PlayerRotationIdle/CPlayerRotationIdle.h"

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
	pPlayer.SetRotation(pPlayer.GetRotation());
}

void CPlayerRotation::Update(CPlayer& pPlayer)
{
	//回転しているかを true に設定.
	pPlayer.SetRotating(true);

	//ローカル軸の取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//クォータニオンの回転を取得.
	D3DXQUATERNION currentQuat = pPlayer.GetQuaternion();

	//目標の角度.
	D3DXQUATERNION targetQuat;
	D3DXQuaternionRotationAxis(&targetQuat, &axes.up, m_RotationDir);

	//現在傾く角度.
	D3DXQUATERNION quat;
	//今の回転角度から目標の回転角度まで滑らかに回転.
	D3DXQuaternionSlerp(&quat, &currentQuat, &targetQuat, m_RotationSpeed);
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの回転を設定.
	pPlayer.SetQuaternion(quat);
}

void CPlayerRotation::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x == 0 && z == 0)
	{
		pPlayer.SetRotationState(std::make_unique<CPlayerRotationIdle>());
		return;
	}

	//最終の回転方向.
	m_RotationDir = atan2(x, z);
}