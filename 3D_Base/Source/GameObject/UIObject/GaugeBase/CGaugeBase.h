#pragma once

#include "GameObject/UIObject/CUIObject.h"

class CGaugeBase
	: public CUIObject
{
public:
	//ゲージの構造体.
	struct Gauge
	{
		float remaining;	//残り.
		float max;			//最大.
	};

public:
	CGaugeBase();
	virtual ~CGaugeBase() override;

	//--- 更新処理 ---.
	virtual void Update() override;

	//--- 描画処理 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj);

	//ワールド座標を設定する関数.
	void SetWorldPos(D3DXVECTOR3 pos) { m_WorldPos = pos; }
	void SetWorldPos(float x, float y, float z) { 
		m_WorldPos.x = x;
		m_WorldPos.y = y;
		m_WorldPos.z = z;
	}

protected:
	//--- ワールド座標を変換する関数 ---.
	D3DXVECTOR3 WorldToScreen(
		const D3DXVECTOR3& worldPos,
		const D3DXMATRIX& view,
		const D3DXMATRIX& proj,
		const D3D11_VIEWPORT& vp);

	void Draw() override final;

protected:
	ComPtr<ID3D11DeviceContext> m_pContext11;	//コンテキスト.

	D3DXVECTOR3		m_WorldPos;		//世界座標.
	D3DXVECTOR3		m_OffsetPos;	//調整座標.
};