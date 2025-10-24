#include "CGaugeBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CGaugeBase::CGaugeBase()
	: m_pContext11		()

	, m_GaugeInfo		()

	, m_WorldPos		( 0.f, 0.f, 0.f )
	, m_OffsetPos		( 0.f, 2.f, 0.f )
{
	Init();
}

//--- 初期化処理 ---.
void CGaugeBase::Init()
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

	//プレイヤーの上に位置調整.
	D3DXVECTOR3 pos = m_WorldPos + m_OffsetPos;
	//2Dに変換.
	D3DXVECTOR3 screenPos = WorldToScreen(pos, View, Proj, vp);

	//画像幅の半分を引いて真ん中にする.
	screenPos.x -= 40.f;

	//変換された位置を設定.
	m_vPosition = D3DXVECTOR3(screenPos.x, screenPos.y, 0.f);

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