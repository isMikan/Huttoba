#pragma once

#include"StaticMesh/CStaticMesh.h"
#include"SkinMesh/CSkinMesh.h"

class MeshManager
{
public:

	static MeshManager* GetInstance()
	{
		static MeshManager instance;
		return &instance;
	}

	HRESULT Create();
	HRESULT LoadData();

	std::shared_ptr<CStaticMesh> GetStaticMesh(StaticMeshList meshId);
	std::shared_ptr<CSkinMesh> GetSkinMesh(SkinMeshList meshId);

private:

	MeshManager();
	~MeshManager();

	MeshManager(MeshManager& shr) = delete;
	MeshManager& operator=(const MeshManager& rhs) = delete;

private:
	
	HRESULT CreateStaticMesh();
	HRESULT CreateSkinMesh();
	HRESULT LoadStaticMesh();
	HRESULT LoadSKinMesh();

private:

	//スタティックメッシュ(使いまわす資源)
	std::unordered_map<StaticMeshList, std::shared_ptr<CStaticMesh>> m_pStaticMeshMap;

	//スキンメッシュ(使いまわす資源)
	std::unordered_map<SkinMeshList, std::shared_ptr<CSkinMesh>> m_pSkinMeshMap;

};