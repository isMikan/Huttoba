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
	CStaticMeshObject();
	virtual ~CStaticMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//メッシュを接続する.
	void AttachMesh(std::shared_ptr<CStaticMesh> pMesh) {
		m_pMesh = pMesh;
	}
	//メッシュを切り離す.
	void DetachMesh() {
		m_pMesh = nullptr;
	}

//=== オブジェクトの色を変更するのに必要.		制作者 [甲把] ===.
	//オブジェクトの色を設定する関数.
	void SetObjectColor(
		int index,
		D3DXVECTOR4& diffuse,
		D3DXVECTOR4& ambient,
		D3DXVECTOR4& specular) {
		//色をまとめる.
		ObjectColor color = { diffuse, ambient, specular };
		m_ObjectColor[index] = color;
	}
	void SetObjectColor(int index, ObjectColor color) {
		m_ObjectColor[index] = color;
	}
	//オブジェクトの色を取得する関数.
	std::vector<ObjectColor> GetObjectColor(int index) const { 
		return std::vector<ObjectColor>{m_ObjectColor[index]};
	}

//===============================================================.

	std::shared_ptr<CStaticMesh> GetMesh() const { return m_pMesh; }

protected:
	std::shared_ptr<CStaticMesh>	m_pMesh;

	std::vector<ObjectColor>		m_ObjectColor;	//オブジェクトの色.		制作者 [甲把]

	std::shared_ptr<CollisionBase>	m_pCollision;	//当たり判定(判定の削除にポインタが必要なので保持)
};
