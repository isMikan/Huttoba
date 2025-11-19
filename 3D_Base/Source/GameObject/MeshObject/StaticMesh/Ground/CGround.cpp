#include "CGround.h"

CGround::CGround()
	: m_FallSpeed				( 1.5f )
	, m_Gravity					( -9.8f )
	, m_Velocity				()

	, m_ChabgeColorSpeed		( 10.f )
	, m_ChabgeColorInterval		( 8.f )
	, m_ShakeSpeed				( 50.f )
	, m_ShakeWidth				( 0.05f )

	, m_ChangeColorTriggerTime	()
	, m_ShakeTriggerTime		()

	, m_IsChangeColor			( false )
	, m_IsShake					( false )
	, m_IsFallDown				( false )
{
	m_DefaultColor =
	{
		D3DXVECTOR4(0.1f, 0.3f, 0.1f, 1.f),
		D3DXVECTOR4(0.1f, 0.2f, 0.1f, 1.f),
		D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f)
	};

	//サイズを指定する.
	m_ObjectColor.resize(1);
	//色を設定.
	m_ObjectColor[0] = m_DefaultColor;
}

CGround::~CGround()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CGround::Update()
{
	CStaticMeshObject::Update();

	//色が変わる場合.
	if (m_IsChangeColor)
	{
		ChangeColorOfGround();
	}
	else
	{
		m_ObjectColor[0] = m_DefaultColor;
	}

	//地面が揺れる場合.
	if (m_IsShake)
	{
		//ShakeGround();
	}

	//地面が落ちる場合.
	if (m_IsFallDown)
	{
		FallDownGround();
	}
}

//--- 描画処理 ---.
void CGround::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- 地面の色が変わる ---.
void CGround::ChangeColorOfGround()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//全体の時間の現在の割合.
	float progress =
		(t - m_ChangeColorTriggerTime) / m_ChangeColorTime;
	progress = std::clamp(progress, 0.f, 1.f);

	D3DXVECTOR4	groundColor = m_ObjectColor[0].diffuse;

	//色の数値.
	float colorChange = cosf(progress * D3DX_PI * m_ChabgeColorSpeed) * m_ChabgeColorInterval;

	//デルタタイムを取得.
	float dt = CTimeManager::GetDeltaTime();

	//点滅.
	groundColor.x += colorChange * dt;
	groundColor.y += colorChange * dt;
	groundColor.z = 0.f;

	m_ObjectColor[0].diffuse = groundColor;
}

//--- 地面が揺れる ---.
void CGround::ShakeGround()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//揺れている時間の現在の割合.
	float progress = 
		(t - m_ShakeTriggerTime) / m_ShakeTime;
	progress = std::clamp(progress, 0.f, 1.f);

	//左右に揺れる.
	m_vPosition.x += cosf(progress * D3DX_PI * m_ShakeSpeed) * m_ShakeWidth;
}

//--- 地面が落ちる ---.
void CGround::FallDownGround()
{
	//デルタタイムを取得.
	float dt = CTimeManager::GetDeltaTime();

	m_Velocity += dt * -m_Gravity;
	m_vPosition.y -= dt * m_Velocity * m_FallSpeed;	//地面を下へ.
}

//--- 数値の領域を指定する関数 ---.
float CGround::Clamp(float value, float min, float max)
{
	//最小値より小さかったら最小値を返す.
	if (value < min) return min;
	//最大値より大きかったら最大値を返す.
	if (value > max) return max;

	return value;
}
