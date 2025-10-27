#pragma once

class CCamera
{
public:
	CCamera();
	~CCamera();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update();

//======================================================================
		
	//カメラの位置を設定.
	void SetPosition(float pos_x, float pos_y, float pos_z ) {
		m_Camera.vPosition.x = pos_x;
		m_Camera.vPosition.y = pos_y;
		m_Camera.vPosition.z = pos_z;
	}

	//カメラの注視点を設定.
	void SetLook(float look_x, float look_y, float look_z ) {
		m_Camera.vLook.x = look_x;
		m_Camera.vLook.y = look_y;
		m_Camera.vLook.z = look_z;
	}

	//ライトの方向を設定.
	void SetLight(float light_x, float light_y, float light_z ) {
		m_Light.vDirection.x = light_x;
		m_Light.vDirection.y = light_y;
		m_Light.vDirection.z = light_z;
	}

	//カメラ情報の取得.
	CAMERA GetCamera() const { return m_Camera; }
	//ライト情報の取得.
	LIGHT GetLight() const { return m_Light; }
	//ビュー(情報の取得.
	D3DXMATRIX GetView() const { return m_mView; }
	//射影情報の取得.
	D3DXMATRIX GetProj() const { return m_mProj; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- ビューの変換処理 ---.
	void UpdateView();
	//--- プロジェクションの変換処理 ---.
	void UpdateProjection();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
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