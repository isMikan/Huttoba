#include "CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

CPlayerActionIdleState::CPlayerActionIdleState()
{
}

CPlayerActionIdleState::~CPlayerActionIdleState()
{
}

void CPlayerActionIdleState::Enter(CPlayer& pPlayer)
{
	pPlayer.SetQuaternion(
		0.f, pPlayer.GetQuaternion().y, 0.f, pPlayer.GetQuaternion().w);
}

void CPlayerActionIdleState::Exit(CPlayer& pPlayer)
{
}

void CPlayerActionIdleState::Update(CPlayer& pPlayer)
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の調整リスト.
	D3DXVECTOR3 offset[]
	{
		rightHandOffset,
		leftHandOffset
	};
	//リストの最大数.
	int offsetMax = sizeof(offset) / sizeof(offset[0]);

	for (int i = 0;i < offsetMax; i++)
	{
		//方向に合わせて位置を調整.
		offset[i] =
			axes.right * offset[i].x +
			axes.up * offset[i].y +
			axes.forward * offset[i].z;

		//手の位置.
		D3DXVECTOR3 handPos = playerPos + offset[i];

		if (i == 0)
		{
			//右手の位置を設定.
			pPlayer.GetPlayerRightHand().SetPosition(handPos);
		}
		else
		{
			//左手の位置を設定.
			pPlayer.GetPlayerLeftHand().SetPosition(handPos);
		}
	}
}
