#include "CGaugeBase.h"

CGaugeBase::CGaugeBase()
	: m_pContext11		()
	, m_WorldPos		( 0.f, 0.f, 0.f )

{
}

CGaugeBase::~CGaugeBase()
{
}

//--- 更新処理 ---.
void CGaugeBase::Update()
{
	CUIObject::Update();
}

//--- 描画処理 ---.
void CGaugeBase::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	D3D11_VIEWPORT vp;	//ビューポート（描画領域）情報を格納.
	UINT num = 1;		//取得するビューポート数.
	m_pContext11 = m_pSprite->GetContext11();
	//ビューポートの取得.
	m_pContext11->RSGetViewports(&num, &vp);

	//変換して設定する.
	D3DXVECTOR3 screenPos = WorldToScreen(m_WorldPos, View, Proj, vp);
	m_pSprite->SetPosition(D3DXVECTOR3(screenPos.x, screenPos.y, 0.f));

	CUIObject::Draw();
}

//--- ワールド座標を変換する関数 ---.
D3DXVECTOR3 CGaugeBase::WorldToScreen(
	const D3DXVECTOR3& worldPos,
	const D3DXMATRIX& view,
	const D3DXMATRIX& proj,
	const D3D11_VIEWPORT& vp)
{
	//ワールド位置 → クリップ座標.
	D3DXVECTOR4 clipPos;
	D3DXMATRIX viewProj = view * proj;
	D3DXVec3Transform(&clipPos, &worldPos, &viewProj);

	//透視除算（-1～1に正規化）.
	clipPos.x /= clipPos.w;
	clipPos.y /= clipPos.w;
	clipPos.z /= clipPos.w;

	//スクリーン座標へ変換(0～).
	D3DXVECTOR3 screenPos;
	screenPos.x = vp.TopLeftX + (clipPos.x + 1.0f) * 0.5f * vp.Width;
	screenPos.y = vp.TopLeftY + (1.0f - clipPos.y) * 0.5f * vp.Height;	//y軸は反転.
	screenPos.z = clipPos.z;

	return screenPos;
}

void CGaugeBase::Draw()
{
}
