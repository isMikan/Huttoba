#pragma once

#include "GameObject/CGameObject.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "Collision/CBoundingSphere.h"

/************************************************************
*	スタティックメッシュオブジェクトクラス.
**/
class CStaticMeshObject
	: public CGameObject
{
public:
	CStaticMeshObject();
	virtual ~CStaticMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//メッシュを接続する.
	void AttachMesh(std::shared_ptr<CStaticMesh> pMesh ){
		m_pMesh = pMesh;
	}
	//メッシュを切り離す.
	void DetachMesh(){
		m_pMesh = nullptr;
	}
	//バウンディングスフィア取得
	CBoundingSphere* GetBSphere() const {
		return m_pBSphere;
	}
	//モデルに合わせたバウンディングスフィア作成のラッパー関数
	HRESULT CreateBSphereForMesh(const std::shared_ptr<CStaticMesh> pMesh) {
		return m_pBSphere->CreateSphereForMesh(pMesh);
	}
	//バウンディングスフィアをオブジェクト位置に合わせる
	//※モデルの原点が中心の場合を想定
	void UpdateBSpherePos() {
		m_pBSphere->SetPosition(m_vPosition + D3DXVECTOR3(0.f,5.f,0.f));
	}

	//モデルの色を設定する関数.
	void SetColor(
		const D3DXVECTOR4& diffuse,
		const D3DXVECTOR4& ambient,
		const D3DXVECTOR4& specular)
	{
		m_Diffuse = diffuse;
		m_Ambient = ambient;
		m_Specular = specular;
	}

protected:
	std::shared_ptr<CStaticMesh>	m_pMesh;
	CBoundingSphere*	m_pBSphere;

	D3DXVECTOR4			m_Diffuse;		//拡散反射.
	D3DXVECTOR4			m_Ambient;		//環境光.
	D3DXVECTOR4			m_Specular;		//鏡面反射.
};
