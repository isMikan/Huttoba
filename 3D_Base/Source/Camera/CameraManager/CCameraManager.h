#pragma once

#include "Camera/CCamera.h"

#include "GameObject/MeshObject/StaticMesh/Ground/GroundManager/CGroundManager.h"

/***********************************************************************
*	カメラマネージャークラス.						制作者 : 甲把
**/
class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	//唯一のインスタンスを作成.
	static CCameraManager& GetInstance()
	{
		static CCameraManager s_Instance;
		return s_Instance;
	}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//更新関数を外部へ.
	static void Update() { GetInstance().Update_Internal(); }
	//更新関数を外部へ.
	static void PositionUpdate(CGroundManager* groundManager) {
		GetInstance().PositionUpdate_Internal(groundManager);
	}
		
//======================================================================

	//カメラの位置を設定.
	static void SetPosition(float pos_x, float pos_y, float pos_z) {
		GetInstance().m_Camera.vPosition.x = pos_x;
		GetInstance().m_Camera.vPosition.y = pos_y;
		GetInstance().m_Camera.vPosition.z = pos_z;
	}

	//カメラの注視点を設定.
	static void SetLook(float look_x, float look_y, float look_z) {
		GetInstance().m_Camera.vLook.x = look_x;
		GetInstance().m_Camera.vLook.y = look_y;
		GetInstance().m_Camera.vLook.z = look_z;
	}

	//ライトの方向を設定.
	static void SetLight(float light_x, float light_y, float light_z) {
		GetInstance().m_Light.vDirection.x = light_x;
		GetInstance().m_Light.vDirection.y = light_y;
		GetInstance().m_Light.vDirection.z = light_z;
	}

	//カメラ情報を取得.
	static CAMERA GetCamera() { return GetInstance().m_Camera; }
	//ライト情報を取得.
	static LIGHT GetLight() { return GetInstance().m_Light; }

	//ビュー行列を取得.
	static D3DXMATRIX GetView() { return GetInstance().m_mView; }
	//プロジェクション行列を取得.
	static D3DXMATRIX GetProjection() { return GetInstance().m_mProj; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update_Internal();
	void PositionUpdate_Internal(CGroundManager* groundManager);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::unique_ptr<CCamera>	m_pCamera;	//カメラ.

	CAMERA			m_Camera;	//カメラ情報.
	LIGHT			m_Light;	//ライト情報.

	D3DXMATRIX		m_mView;	//ビュー.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）.

	std::vector<CAMERA>		m_LastCamera;	//目的のカメラ情報.
};