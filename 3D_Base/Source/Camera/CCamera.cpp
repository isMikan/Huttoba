#include "CCamera.h"

CCamera::CCamera()
	: m_vUpVec	( 0.f, 1.f, 0.f )

	, m_Fov_y	( D3DXToRadian( 45.f ) )	//数値を大きくしたら視野が狭くなる.
	, m_Aspect	()
	, m_Near_z	( 0.1f )
	, m_Far_z	( 100.f )
{
}

CCamera::~CCamera()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- ビューの変換処理 ---.
void CCamera::UpdateView(const CAMERA& camera, D3DXMATRIX& view) const
{
	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&view,		//(out)ビュー計算結果.
		&camera.vPosition,
		&camera.vLook,
		&m_vUpVec);
}

//--- プロジェクションの変換処理 ---.
void CCamera::UpdateProjection(D3DXMATRIX& proj)
{
	//アスペクト（幅÷高さ）.
	m_Aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&proj,		//(out)プロジェクション計算結果.
		m_Fov_y,	//視野角（FOV：Field of View）.
		m_Aspect,	//アスペクト.
		m_Near_z,	//近いビュー平面のz値.
		m_Far_z);	//遠いビュー平面のz値.
}
