#pragma once

#include "GameObject/UIObject/CUIObject.h"

class CGaugeBase
	: public CUIObject
{
public:
	CGaugeBase();
	~CGaugeBase();

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

protected:
	//--- ワールド座標を変換する関数 ---.
	D3DXVECTOR3 WorldToScreen(
		const D3DXVECTOR3& worldPos,
		const D3DXMATRIX& view,
		const D3DXMATRIX& proj,
		const D3D11_VIEWPORT& vp);

protected:
	ComPtr<ID3D11DeviceContext> m_pContext11;	//コンテキスト.

	D3DXVECTOR3		m_WorldPos;		//世界座標.
};