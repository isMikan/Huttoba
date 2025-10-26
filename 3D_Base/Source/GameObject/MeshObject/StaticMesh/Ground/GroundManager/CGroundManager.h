#pragma once

#include "GameObject/MeshObject/StaticMesh/Ground/CGround.h"

/***********************************************************************
*	地面マネージャークラス.
**/
class CGroundManager
{
public:
	enum GroundType
	{
		FirstFallGround = 0,	//一番目に落ちる地面.
		SecondFallGround,		//二番目に落ちる地面.
		ThirdFallGround,		//三番目に落ちる地面.
		SafeGround,				//落ちない中央の地面.

		None = -1				//なし.
	};

public:
	CGroundManager();
	~CGroundManager();

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

protected:
	std::vector<std::unique_ptr<CGround>>	m_pGrounds;	//地面.

	std::array<float, Ground_Max>	m_FallTime;		//落ちる時間.

};