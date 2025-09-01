#pragma once
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

class ItemBase;

class ItemManager:
	public CStaticMeshObject
{
public:
	ItemManager();
	~ItemManager();



	HRESULT Create();
	HRESULT LoadData();
	void Init();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

private:
	std::vector<std::unique_ptr<ItemBase>> m_pItems;

};