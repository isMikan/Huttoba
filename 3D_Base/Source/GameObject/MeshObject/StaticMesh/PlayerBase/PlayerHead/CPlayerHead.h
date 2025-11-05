#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/***********************************************************************
*	プレイヤーの頭クラス.
**/
class CPlayerHead
	: public CStaticMeshObject	//キャラクタークラスを継承.
{
public:
	CPlayerHead();
	~CPlayerHead();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	void Update(D3DXQUATERNION quat);
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
	
	//======================================================================
		
	//頭の位置の調整を取得.
	D3DXVECTOR3 GetOffsetPos() const { return m_OffsetPos; }

private:
	//使用しない override 関数.
	void Update() override {};

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	D3DXVECTOR3	m_OffsetPos;	//位置の調整.

};