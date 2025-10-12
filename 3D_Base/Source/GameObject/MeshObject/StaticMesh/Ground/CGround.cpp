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
	float t = CTimeManager::GetTotalTime();

	//全体の時間の現在の割合.
	float progress =
		(t - m_ChangeColorStartTime) / (m_ChangeColorStartTime + 5.f);

	D3DXVECTOR4	groundColor = m_ObjColor.diffuse;

	//点滅.
	groundColor.x += cosf(progress * D3DX_PI * 10.f);

	m_ObjColor.diffuse = groundColor;
}

//--- 地面が揺れる ---.
void CGround::VibrateGround()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//揺れている時間の現在の割合.
	float progress = 
		(t - m_VibrateStartTime) / (m_VibrateStartTime + m_VibrateTime);

	//左右に揺れる.
	m_vPosition.x += cosf(progress * D3DX_PI * 50.f) * 0.01;
}

//--- 地面が落ちる ---.
void CGround::FallDownGround()
{
	//デルタタイムを取得.
	float dt = CTimeManager::GetDeltaTime();

	m_vPosition.y -= dt * m_FallSpeed;	//地面を下へ.
}
