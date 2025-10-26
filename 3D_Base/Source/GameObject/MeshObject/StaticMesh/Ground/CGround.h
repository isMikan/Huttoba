#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/***********************************************************************
*	地面クラス.
**/
class CGround
	: public CStaticMeshObject
{
public:
//======================================================================
// 	   外部で呼び出す変数.
//======================================================================
	//色が変わる時間.
	static constexpr float	m_ChangeColorTime = 5.f;	
	//揺れる時間.
	static constexpr float	m_ShakeTime = 2.f;

public:
	CGround();
	~CGround();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update() override;

	//--- 描画処理 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	//地面の色が変わるか取得と設定.
	bool GetIsChangeColor() const { return m_IsChangeColor; }
	void SetIsChangeColor(bool change) { m_IsChangeColor = change; }
	//地面が揺れるか取得と設定.
	bool GetIsShake() const { return m_IsShake; }
	void SetIsShake(bool vibrate) { m_IsShake = vibrate; }
	//地面が落ちるか取得と設定.
	bool GetIsFallDown() const { return m_IsFallDown; }
	void SetIsFallDown(bool fall) { m_IsFallDown = fall; }

	//地面の色が変わり始めた時間を設定.
	void SetChangeColorTriggerTime(float time) { m_ChangeColorTriggerTime = time; }
	//地面が揺れ始めた時間を設定.
	void SetShakeTriggerTime(float time) { m_ShakeTriggerTime = time; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 地面の色が変わる ---.
	void ChangeColorOfGround();

	//--- 地面が揺れる ---.
	void ShakeGround();

	//--- 地面が落ちる ---.
	void FallDownGround();

	//--- 数値の領域を指定する関数 ---.
	float Clamp(float value, float min, float max);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	ObjectColor	m_DefaultColor;		//初期色.

	float	m_FallSpeed;			//落下速度.
	float	m_Gravity;				//重力.
	float	m_Velocity;				//重力速度.

	float	m_ChabgeColorSpeed;		//色を変える速度.
	float	m_ChabgeColorInterval;	//色を変える間隔.
	float	m_ShakeSpeed;			//揺れる速度.
	float	m_ShakeWidth;			//揺れる幅.

	float	m_ChangeColorTriggerTime;	//色が変わり始めた時間.
	float	m_ShakeTriggerTime;		//揺れ始めた時間.

	bool	m_IsChangeColor;		//色が変わるか.
	bool	m_IsShake;				//揺れるか.
	bool	m_IsFallDown;			//落ちるか.
};