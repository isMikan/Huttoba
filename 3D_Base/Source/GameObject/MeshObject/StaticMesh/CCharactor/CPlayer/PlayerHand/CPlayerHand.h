#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"

#include <memory>

/**************************************************
*	プレイヤーの手クラス.
**/
class CPlayerHand
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	CPlayerHand();
	virtual ~CPlayerHand() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//手の位置の調整を取得.
	D3DXVECTOR3 GetOffsetPos() const { return m_OffsetPos; }

protected:
	D3DXVECTOR3	m_OffsetPos;	//位置の調整.
};