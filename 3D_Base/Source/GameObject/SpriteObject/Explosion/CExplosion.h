#pragma once
#include "GameObject/SpriteObject/CSpriteObject.h"

/***********************************************************************
*	爆発クラス
**/
class CExplosion
	: public CSpriteObject	//スプライトオブジェクトクラスを継承
{
public:
	CExplosion();
	virtual ~CExplosion() override;

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;
	bool IsStart() const { return m_StartFlag; };

	void Start(const D3DXVECTOR3& osition);

protected:
	POINTS	m_PatternNo;	//パターン番号(マス目)
	int		m_AnimCount;	//アニメーションカウント

	bool	m_StartFlag;	//trueならUpdateを回すフラッグ

private:

	void ResetAnimation();

};
