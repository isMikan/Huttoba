#pragma once

#include "GameObject/CGameObject.h"
#include "Assets/Mesh/SkinMesh/CSkinMesh.h"

/************************************************************
*	スキンメッシュオブジェクトクラス.
**/
class CSkinMeshObject
	: public CGameObject
{
public:
	CSkinMeshObject();
	virtual ~CSkinMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//メッシュを接続する.
	void AttachMesh(std::shared_ptr<CSkinMesh> pMesh);
	//メッシュを切り離す.
	void DetachMesh();

protected:
	std::shared_ptr<CSkinMesh>		m_pMesh;
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;
};
