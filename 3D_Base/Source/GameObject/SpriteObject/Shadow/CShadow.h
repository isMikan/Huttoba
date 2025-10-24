#pragma once

#include "GameObject/SpriteObject/CSpriteObject.h"

/**************************************************
*	影クラス
**/
class CShadow
	: public CSpriteObject	//スプライトオブジェクトクラスを継承
{
public:
	CShadow();
	~CShadow() override;

	void Update(D3DXVECTOR3 objectPos);
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

protected:
	//使用しない override 関数.
	void Update() override {};

};
