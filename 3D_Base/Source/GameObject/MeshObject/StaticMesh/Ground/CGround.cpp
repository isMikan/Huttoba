#include "CGround.h"

CGround::CGround()
	: m_FallSpeed				( 10.f )

	, m_ChangeColorStartTime	()
	, m_VibrateStartTime		()

	, m_IsChangeColor			( false )
	, m_IsVibrate				( false )
	, m_IsFallDown				( false )
{
	m_ObjColor.diffuse =	
		D3DXVECTOR4( 0.1f, 0.3f, 0.1f, 1.f );
	m_ObjColor.ambient =
		D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );
	m_ObjColor.specular =
		D3DXVECTOR4( 0.1f, 0.1f, 0.1f, 1.f );
}

CGround::~CGround()
{
}

//--- 更新処理 ---.
void CGround::Update()
{
	CStaticMeshObject::Update();

	//色が変わる場合.
	if (m_IsChangeColor)
	{
		ChangeColorOfGround();
	}

	//地面が揺れる場合.
	if (m_IsVibrate)
	{
		VibrateGround();
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

//--- 地面の色が変わる ---.
void CGround::ChangeColorOfGround()
{
	//経過時間を取得.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//全体の時間の現在の割合.
	float progress =
		(t - m_ChangeColorStartTime) / (m_ChangeColorStartTime + 5.f);

	D3DXVECTOR4	groundColor = m_ObjColor.diffuse;

	//点滅.
	groundColor.x += cosf(progress * D3DX_PI * 12.f) * 0.1f;
	groundColor.y += cosf(progress * D3DX_PI * 12.f) * 0.1f;
	groundColor.z = 0.f;
	progress = Clamp(progress, 0.f, 1.f);

	m_ObjColor.diffuse = groundColor;
}

//--- 地面が揺れる ---.
void CGround::VibrateGround()
{
	//経過時間を取得.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//揺れている時間の現在の割合.
	float progress = 
		(t - m_VibrateStartTime) / (m_VibrateStartTime + m_VibrateTime);
	progress = Clamp(progress, 0.f, 1.f);

	//左右に揺れる.
	m_vPosition.x += cosf(progress * D3DX_PI * 50.f) * 0.02;
}

//--- 地面が落ちる ---.
void CGround::FallDownGround()
{
	//デルタタイムを取得.
	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	m_vPosition.y -= dt * m_FallSpeed;	//地面を下へ.
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
