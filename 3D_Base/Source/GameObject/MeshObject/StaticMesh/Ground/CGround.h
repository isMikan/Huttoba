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
	static constexpr float	m_ChangeColorTime = 3.f;	
	//揺れる時間.
	static constexpr float	m_VibrateTime = 1.f;

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

private:
	float	m_FallSpeed;			//落下速度.

	float	m_ChangeColorStartTime;	//色が変わり始めた時間.
	float	m_VibrateStartTime;		//揺れ始めた時間.

	bool	m_IsChangeColor;		//色が変わるか.
	bool	m_IsVibrate;			//揺れるか.
	bool	m_IsFallDown;			//落ちるか.
};