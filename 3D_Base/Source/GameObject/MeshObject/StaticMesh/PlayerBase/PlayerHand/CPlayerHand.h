#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/***********************************************************************
*	プレイヤーの手クラス.
**/
class CPlayerHand
	: public CStaticMeshObject	//キャラクタークラスを継承.
{
public:
	CPlayerHand();
	virtual ~CPlayerHand() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	virtual void Update() override;
	//--- 毎フレームの描画 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

//======================================================================
		
	//手の位置の調整を取得.
	D3DXVECTOR3 GetOffsetPos() const { return m_OffsetPos; }

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	D3DXVECTOR3	m_OffsetPos;	//位置の調整.

};