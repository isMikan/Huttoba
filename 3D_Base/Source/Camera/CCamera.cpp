#include "CCamera.h"

CCamera::CCamera()
	: m_Camera	()
	, m_Light	()

	, m_mView	()
	, m_mProj	()

	, m_vUpVec	( 0.f, 1.f, 0.f )

	, m_Fov_y	( D3DXToRadian( 45.f ) )	//数値を大きくしたら視野が狭くなる.
	, m_Aspect	()
	, m_Near_z	( 0.1f )
	, m_Far_z	( 100.f )
{
}

CCamera::~CCamera()
{
}

void CCamera::Update()
{
	UpdateView();
	UpdateProjection();
}

void CCamera::UpdateView()
{
	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&m_Camera.vPosition, 
		&m_Camera.vLook, 
		&m_vUpVec);
}

void CCamera::UpdateProjection()
{
	//アスペクト（幅÷高さ）.
	m_Aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		m_Fov_y,	//視野角（FOV：Field of View）.
		m_Aspect,	//アスペクト.
		m_Near_z,	//近いビュー平面のz値.
		m_Far_z);	//遠いビュー平面のz値.
}
