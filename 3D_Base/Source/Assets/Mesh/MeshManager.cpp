#include "stdafx.h"
#include "MeshManager.h"


MeshManager::MeshManager()
	: m_pStaticMeshMap	()
{
}

MeshManager::~MeshManager()
{
}

HRESULT MeshManager::Create()
{
	if (CreateStaticMesh()	!= S_OK) { return E_FAIL; }
	if (CreateSkinMesh()	!= S_OK) { return E_FAIL; }
	return S_OK;
}

HRESULT MeshManager::LoadData()
{
	if (LoadStaticMesh()	!= S_OK) { return E_FAIL; }
	if (LoadSKinMesh()		!= S_OK) { return E_FAIL; }
	return S_OK;
}

std::shared_ptr<CStaticMesh> MeshManager::GetStaticMesh(StaticMeshList meshId)
{
	auto it = m_pStaticMeshMap.find(meshId);
	if (it != m_pStaticMeshMap.end())
	{
		return it->second;
	}
	return nullptr;
}

std::shared_ptr<CSkinMesh> MeshManager::GetSkinMesh(SkinMeshList meshId)
{
	auto it = m_pSkinMeshMap.find(meshId);
	if (it != m_pSkinMeshMap.end())
	{
		return it->second;
	}
	return nullptr;
}

HRESULT MeshManager::CreateStaticMesh()
{
	StaticMeshList MeshList[] =
	{
		StaticMeshList::Player,
		StaticMeshList::Ground,
		StaticMeshList::RoboA,
		StaticMeshList::RoboB,
		StaticMeshList::Bullet,
		StaticMeshList::BSphere,
	};

	for (auto& id : MeshList)
	{
		m_pStaticMeshMap[id] = std::make_unique<CStaticMesh>();
		if (!m_pStaticMeshMap[id]) E_POINTER;
	}

	return S_OK;
}

HRESULT MeshManager::CreateSkinMesh()
{
	SkinMeshList skinMesh[] =
	{
		SkinMeshList::Zako
	};

	//スキンメッシュのインスタンス作成
	for (auto& id : skinMesh)
	{
		m_pSkinMeshMap[id] = std::make_unique<CSkinMesh>();
		if (!m_pSkinMeshMap[id]) return E_POINTER;
	}

	return S_OK;
}

HRESULT MeshManager::LoadStaticMesh()
{
	//スタティックメッシュの読み込み
	m_pStaticMeshMap[StaticMeshList::Player ]->Init(_T("Data\\Mesh\\Static\\Character\\character.x"));
	m_pStaticMeshMap[StaticMeshList::Ground	]->Init(_T("Data\\Mesh\\Static\\Ground\\ground.x"));
	m_pStaticMeshMap[StaticMeshList::RoboA	]->Init(_T("Data\\Mesh\\Static\\Robo\\RobotA_pivot.x"));
	m_pStaticMeshMap[StaticMeshList::RoboB	]->Init(_T("Data\\Mesh\\Static\\Robo\\RobotB_pivot.x"));
	m_pStaticMeshMap[StaticMeshList::Bullet ]->Init(_T("Data\\Mesh\\Static\\Bullet\\bullet.x"));

	//バウンディングスフィア(当たり判定用)
	m_pStaticMeshMap[StaticMeshList::BSphere]->Init(_T("Data\\Collision\\Sphere.x"));

	return S_OK;
}

HRESULT MeshManager::LoadSKinMesh()
{
	//スキンメッシュの読み込み
	m_pSkinMeshMap[SkinMeshList::Zako]->Init(_T("Data\\Mesh\\Skin\\zako\\zako.x"));

	return S_OK;
}

