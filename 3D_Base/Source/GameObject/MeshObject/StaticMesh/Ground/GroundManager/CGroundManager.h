#pragma once

#include "Ground/CGround.h"

/***********************************************************************
*	地面マネージャークラス.							制作者 [甲把]
**/
class CGroundManager
{
public:
//======================================================================
// 	   列挙型.
//======================================================================
	enum GroundType
	{
		SafeGround = 0,		//落ちない中央の地面.
		ThirdFallGround,	//三番目に落ちる地面.
		SecondFallGround,	//二番目に落ちる地面.
		FirstFallGround,	//一番目に落ちる地面.

		None = -1			//なし.
	};

public:
	CGroundManager();
	~CGroundManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 構築関数 ---.
	void Create();
	//--- 読込関数 ---.
	void LoadData();
	//--- 破棄関数 ---.
	void Destroy();
	//--- 更新関数 ---.
	void Update();
	//--- 描画関数 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

	//地面を取得.
	const std::vector<std::unique_ptr<CGround>>& GetGrounds() const { return m_pGrounds; }

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CGround>>	m_pGrounds;	//地面.

	std::array<float, Ground_Max>	m_FallTime;		//落ちる時間.

};