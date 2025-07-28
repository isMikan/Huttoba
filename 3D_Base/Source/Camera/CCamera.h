#pragma once

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update();

	//カメラ関数.
	void UpdateView();
	//プロジェクション関数.
	void UpdateProjection();

	CAMERA GetCamera() const { return m_Camera; }
	LIGHT GetLight() const { return m_Light; }
	D3DXMATRIX GetView() const { return m_mView; }
	D3DXMATRIX GetProj() const { return m_mProj; }

private:
	CAMERA			m_Camera;	//カメラ情報.
	LIGHT			m_Light;	//ライト情報.

	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.

	D3DXVECTOR3		m_vUpVec;	//上方（ベクトル）.	

	float			m_Fov_y;	//y方向の視野角.
	float			m_Aspect;	//アスペクト.
	float			m_Near_z;	//近いビュー平面のz値.
	float			m_Far_z;	//遠いビュー平面のz値.
};