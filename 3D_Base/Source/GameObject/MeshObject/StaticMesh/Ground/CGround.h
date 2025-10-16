#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/**************************************************
*	地面クラス.
**/
class CGround
	: public CStaticMeshObject
{
public:
	//色が変わる時間.
	static constexpr float	m_ChangeColorTime = 5.f;	
	//揺れる時間.
	static constexpr float	m_VibrateTime = 2.f;

public:
	CGround();
	~CGround();

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
	bool GetIsVibrate() const { return m_IsVibrate; }
	void SetIsVibrate(bool vibrate) { m_IsVibrate = vibrate; }
	//地面が落ちるか取得と設定.
	bool GetIsFallDown() const { return m_IsFallDown; }
	void SetIsFallDown(bool fall) { m_IsFallDown = fall; }

	//地面の色が変わり始めた時間を設定.
	void SetChangeColorStartTime(float time) { m_ChangeColorStartTime = time; }
	//地面が揺れ始めた時間を設定.
	void SetVibrateStartTime(float time) { m_VibrateStartTime = time; }

private:
	//--- 地面の色が変わる ---.
	void ChangeColorOfGround();

	//--- 地面が揺れる ---.
	void VibrateGround();

	//--- 地面が落ちる ---.
	void FallDownGround();

	//--- 数値の領域を指定する関数 ---.
	float Clamp(float value, float min, float max);

private:
	ObjectColor	m_DefaultColor;		//初期色.

	float	m_FallSpeed;			//落下速度.
	float	m_Gravity;				//重力.
	float	m_Velocity;				//重力速度.

	float	m_ChabgeColorSpeed;		//色を変える速度.
	float	m_ChabgeColorInterval;	//色を変える間隔.
	float	m_VibrateSpeed;			//揺れる速度.
	float	m_VibrateWidth;			//揺れる幅.

	float	m_ChangeColorStartTime;	//色が変わり始めた時間.
	float	m_VibrateStartTime;		//揺れ始めた時間.

	bool	m_IsChangeColor;		//色が変わるか.
	bool	m_IsVibrate;			//揺れるか.
	bool	m_IsFallDown;			//落ちるか.
};