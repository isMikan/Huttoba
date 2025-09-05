#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"

/**************************************************
*	プレイヤーの頭クラス.
**/
class CPlayerHead
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	CPlayerHead();
	~CPlayerHead();

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
	
	//頭の位置の調整を取得.
	D3DXVECTOR3 GetOffsetPos() const { return m_OffsetPos; }

private:
	D3DXVECTOR3	m_OffsetPos;	//位置の調整.
};