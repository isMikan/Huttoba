#pragma once

#include "GameObject/CGameObject.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"

#include "GameObject/SpriteObject/Shadow/CShadow.h"

/************************************************************
*	スタティックメッシュオブジェクトクラス.
**/
class CStaticMeshObject
	: public CGameObject
{
public:
	//オブジェクトの色の構造体.
	struct ObjectColor
	{
		D3DXVECTOR4	diffuse;	//拡散反射.
		D3DXVECTOR4	ambient;	//環境光.
		D3DXVECTOR4	specular;	//鏡面反射.
	};

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

	//オブジェクトの色を設定する関数.
	void SetObjectColor(
		D3DXVECTOR4& diffuse, D3DXVECTOR4& ambient, D3DXVECTOR4& specular){
		m_ObjColor.diffuse = diffuse;
		m_ObjColor.ambient = ambient;
		m_ObjColor.specular = specular;
	}
	void SetObjectColor(ObjectColor color){
		m_ObjColor = color;
	}
	//オブジェクトの色を取得する関数.
	ObjectColor GetObjectColor() const { return m_ObjColor; }

protected:
	std::shared_ptr<CStaticMesh>	m_pMesh;

	ObjectColor				m_ObjColor;		//オブジェクトの色.
};
