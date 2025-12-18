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
	//--- 読込関数 ---.
	void LoadData();
	//--- 更新関数 ---.
	void Update();
	//--- 描画関数 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

//=== 各シーンの構築関数 ===.
	//--- メイン ---.
	void MainGroundCreate();
	//--- リザルト ---.
	void ResultGroundCreate();

//======================================================================
	
	//地面を取得.
	const std::vector<std::unique_ptr<CGround>>& GetGrounds() const { return m_pGrounds; }

	//一番外のステージの色が変わっているかを取得(sizeだと要素数を返すので-1している)
	bool GetIsFallDown() { return m_pGrounds[m_pGrounds.size() - 1]->GetIsChangeColor(); }

	//現在の地面の半径を取得
	float GetGroundRadius() 
	{
		//地面が光始めるか、地面が落ちている最中なら
		if (GetIsFallDown()|| m_pGrounds[m_pGrounds.size() - 1]->GetIsFallDown())
		{
			//地面の配列の数が2以上なら
			if (m_pGrounds.size() >= 2)
			{
				return m_GroundRadius[m_pGrounds.size() - 2];
			}
		}
		 
		//if文をの中を通らなければ現在のステージの半径を返す
		return m_GroundRadius[m_pGrounds.size() - 1];
	}

	//地面の中心位置を取得
	D3DXVECTOR3 GetGroundCenterPos();

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 構築関数 ---.
	void Create();
	//--- 破棄関数 ---.
	void Destroy(int index);

	//地面の半径を抽出
	bool ExtractMeshRadius();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CGround>>	m_pGrounds;	//地面.

	std::array<float, Ground_Max>	m_FallTime;		//落ちる時間.

	//地面の半径
	std::array<float, Ground_Max>	m_GroundRadius;
};