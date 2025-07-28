#include "CExplosion.h"

static constexpr int animInterval = 3;

CExplosion::CExplosion()
	: m_PatternNo()
	, m_AnimCount()
	, m_StartFlag()
{
}

CExplosion::~CExplosion()
{
}

void CExplosion::Update()
{
	//アニメーションを初期化
	const POINTS PatternMax = m_pSprite->GetPatternMax();

	m_AnimCount++;
	//アニメインターバルごとに画像切り替え
	if (m_AnimCount % animInterval == 0) {
		m_PatternNo.x = (m_AnimCount / animInterval) % PatternMax.x;
		m_PatternNo.y = (m_AnimCount / animInterval) / PatternMax.y;
	}
	if (m_AnimCount / animInterval >= PatternMax.x * PatternMax.y) {
		m_AnimCount = 0;
		m_StartFlag = false;
	}
}

void CExplosion::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_pSprite->SetPatternNo(m_PatternNo.x, m_PatternNo.y);

	m_pSprite->SetBillboard(true);

	CSpriteObject::Draw(View, Proj);

	m_pSprite->SetBillboard(false);
}

void CExplosion::Start(const D3DXVECTOR3& position)
{
	m_StartFlag = true;
	ResetAnimation();
	m_vPosition = position;
}

void CExplosion::ResetAnimation()
{
	m_PatternNo.x = 0;
	m_PatternNo.y = 0;
	m_AnimCount = 0;
}
