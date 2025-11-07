#pragma once

#include "GameObject/UIObject/CUIObject.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/***********************************************************************
*	ゲージベースクラス.							制作者 [甲把]
**/
class CGaugeBase
	: public CUIObject
{
public:
	CGaugeBase();
	virtual ~CGaugeBase() = default;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 初期化処理 ---.
	virtual void Init();
	//--- 更新処理 ---.
	virtual void Update() = 0;
	//--- 描画処理 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj) = 0;

	//ワールド座標を設定する関数.
	void SetWorldPos(D3DXVECTOR3 pos) { m_WorldPos = pos; }
	void SetWorldPos(float x, float y, float z) { 
		m_WorldPos.x = x;
		m_WorldPos.y = y;
		m_WorldPos.z = z;
	}

	//ゲージの必要情報を設定.
	void SetGaugeInfo(Gauge gauge) { m_GaugeInfo = gauge; }

protected:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- ワールド座標を変換する関数 ---.
	D3DXVECTOR3 WorldToScreen(
		const D3DXVECTOR3& worldPos,
		const D3DXMATRIX& view,
		const D3DXMATRIX& proj,
		const D3D11_VIEWPORT& vp);

	//使用しない override 関数(これ以降も使用しない).
	void Draw() override final {};

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	ComPtr<ID3D11DeviceContext>		m_pContext11;	//コンテキスト.

	Gauge			m_GaugeInfo;	//ゲージの情報.

	D3DXVECTOR3		m_WorldPos;		//世界座標.
	D3DXVECTOR3		m_OffsetPos;	//調整座標.

};