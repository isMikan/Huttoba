#include "stdafx.h"
#include "MeshManager.h"
MeshManager::MeshManager()
	: m_pStaticMeshMap	()
	, m_pSkinMeshMap()
	, m_StaticMeshInfoList()
	, m_SkinMeshInfoList()
{
}

MeshManager::~MeshManager()
{
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::Create()
{
	if (CreateStaticMesh()	!= S_OK) { return E_FAIL; }
	if (CreateSkinMesh()	!= S_OK) { return E_FAIL; }
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::LoadData()
{
	if (LoadStaticMesh()	!= S_OK) { return E_FAIL; }
	if (LoadSKinMesh()		!= S_OK) { return E_FAIL; }
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

std::shared_ptr<CStaticMesh> MeshManager::GetStaticMesh(StaticMeshList meshId)
{
	auto it = m_pStaticMeshMap.find(meshId);
	if (it != m_pStaticMeshMap.end())
	{
		return it->second;
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------

std::shared_ptr<CSkinMesh> MeshManager::GetSkinMesh(SkinMeshList meshId)
{
	auto it = m_pSkinMeshMap.find(meshId);
	if (it != m_pSkinMeshMap.end())
	{
		return it->second;
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::CreateStaticMesh()
{
	//各staticMeshの情報を登録
//↓-----------------------------------------------------------------------------------------------------------------------------↓
	RegisterStaticMesh(StaticMeshList::FirstFallGround,	_T("Data\\Mesh\\Static\\Ground\\fall_floor_1.x"));
	RegisterStaticMesh(StaticMeshList::SecondFallGround,_T("Data\\Mesh\\Static\\Ground\\fall_floor_2.x"));
	RegisterStaticMesh(StaticMeshList::ThirdFallGround,	_T("Data\\Mesh\\Static\\Ground\\fall_floor_3.x"));
	RegisterStaticMesh(StaticMeshList::SafeGround,		_T("Data\\Mesh\\Static\\Ground\\safe_floor.x"));
	RegisterStaticMesh(StaticMeshList::Player,			_T("Data\\Mesh\\Static\\Character\\character.x"));
	RegisterStaticMesh(StaticMeshList::PBody,			_T("Data\\Mesh\\Static\\Character\\body.x"));
	RegisterStaticMesh(StaticMeshList::PHead,			_T("Data\\Mesh\\Static\\Character\\head.x"));
	RegisterStaticMesh(StaticMeshList::PHand,			_T("Data\\Mesh\\Static\\Character\\hand.x"));
	RegisterStaticMesh(StaticMeshList::RoboA,			_T("Data\\Mesh\\Static\\Robo\\RobotA_pivot.x"));
	RegisterStaticMesh(StaticMeshList::RoboB,			_T("Data\\Mesh\\Static\\Robo\\RobotB_pivot.x"));
	RegisterStaticMesh(StaticMeshList::Bullet,			_T("Data\\Mesh\\Static\\Bullet\\bullet.x"));
	RegisterStaticMesh(StaticMeshList::BSphere,			_T("Data\\Collision\\Sphere.x"));
	RegisterStaticMesh(StaticMeshList::BCapsule,		_T("Data\\Collision\\Capsule.x"));
	RegisterStaticMesh(StaticMeshList::BWidthCapsule,	_T("Data\\Collision\\WidthCapsule.x"));
	RegisterStaticMesh(StaticMeshList::Haetataki,		_T("Data\\Mesh\\Static\\Item\\pikopiko.x"));
	RegisterStaticMesh(StaticMeshList::Bomb,			_T("Data\\Mesh\\Static\\Item\\Bomb.x"));
	RegisterStaticMesh(StaticMeshList::Fun,				_T("Data\\Mesh\\Static\\Item\\Fun.x"));
	RegisterStaticMesh(StaticMeshList::Mushroom,		_T("Data\\Mesh\\Static\\Item\\Mushroom.x"));
	//RegisterStaticMesh(StaticMeshList::TrackingRobot,	_T("Data\\Mesh\\Skin\\zako\\zako.x"));
	RegisterStaticMesh(StaticMeshList::TrackingRobot,	_T("Data\\Mesh\\Static\\Item\\TrackingRobot.x"));
	RegisterStaticMesh(StaticMeshList::Boomerang,		_T("Data\\Mesh\\Static\\Item\\Boomerang.x"));
	RegisterStaticMesh(StaticMeshList::SmashBat,		_T("Data\\Mesh\\Static\\Item\\SmashBat.x"));

	RegisterStaticMesh(StaticMeshList::ExplosionCol,	_T("Data\\Mesh\\Static\\Collision\\ExplosionCol.x"));
	RegisterStaticMesh(StaticMeshList::FunCol,			_T("Data\\Mesh\\Static\\Collision\\testCapsule.x"));
	RegisterStaticMesh(StaticMeshList::TestPlayer,		_T("Data\\Mesh\\Static\\Collision\\TestPlayer.x"));
	RegisterStaticMesh(StaticMeshList::BoomerangCol,	_T("Data\\Mesh\\Static\\Collision\\BoomerangCol.x"));
	RegisterStaticMesh(StaticMeshList::PickUpCol,		_T("Data\\Mesh\\Static\\Collision\\PickUpCol.x"));
	RegisterStaticMesh(StaticMeshList::HaetatakiPickUpCol,		_T("Data\\Mesh\\Static\\Collision\\PickUpHaetatakiCol.x"));
	RegisterStaticMesh(StaticMeshList::HaetatakiCol,	_T("Data\\Mesh\\Static\\Collision\\HaetatakiCol.x"));
	RegisterStaticMesh(StaticMeshList::PlayerCol,		_T("Data\\Mesh\\Static\\Collision\\PlayerCol.x"));
	RegisterStaticMesh(StaticMeshList::SmashBatCol,		_T("Data\\Mesh\\Static\\Collision\\SmashBatCol.x"));

	RegisterStaticMesh(StaticMeshList::floor1,			_T("Data\\Mesh\\Static\\Collision\\floor1.x"));
	RegisterStaticMesh(StaticMeshList::floor2,			_T("Data\\Mesh\\Static\\Collision\\floor2.x"));
	RegisterStaticMesh(StaticMeshList::floor3,			_T("Data\\Mesh\\Static\\Collision\\floor3.x"));
	RegisterStaticMesh(StaticMeshList::floor4,			_T("Data\\Mesh\\Static\\Collision\\floor4.x"));

//↑-----------------------------------------------------------------------------------------------------------------------------↑

	for (auto& mesh : m_StaticMeshInfoList)
	{
		m_pStaticMeshMap[mesh.Id] = std::make_unique<CStaticMesh>();
		if (!m_pStaticMeshMap[mesh.Id]) E_POINTER;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::CreateSkinMesh()
{
	//各SkinMeshの情報を登録
	//↓-----------------------------------------------------------------------------------------------------------------------------↓
	RegisterSkinMesh(SkinMeshList::Zako,	_T("Data\\Mesh\\Skin\\zako\\zako.x"));
	RegisterSkinMesh(SkinMeshList::Chick,	_T("Data\\Mesh\\Skin\\hiyoko\\ef_hiyoko.x"));
	//↑-----------------------------------------------------------------------------------------------------------------------------↑

	//スキンメッシュのインスタンス作成
	for (auto& mesh : m_SkinMeshInfoList)
	{
		m_pSkinMeshMap[mesh.Id] = std::make_unique<CSkinMesh>();
		if (!m_pSkinMeshMap[mesh.Id]) return E_POINTER;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::LoadStaticMesh()
{
	for(auto& mesh : m_StaticMeshInfoList)
	{
		auto it = m_pStaticMeshMap.find(mesh.Id);
		if (it != m_pStaticMeshMap.end())
		{
			it->second->Init(mesh.Path);
		}
		else
		{
			return E_FAIL; // メッシュが見つからない場合はエラー
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT MeshManager::LoadSKinMesh()
{
	for (auto& mesh : m_SkinMeshInfoList)
	{
		auto it = m_pSkinMeshMap.find(mesh.Id);
		if (it != m_pSkinMeshMap.end())
		{
			it->second->Init(mesh.Path);
		}
		else
		{
			return E_FAIL; // メッシュが見つからない場合はエラー
		}
	}
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

void MeshManager::RegisterStaticMesh(StaticMeshList staticMeshID, LPCTSTR path)
{
	m_StaticMeshInfoList.push_back({ staticMeshID, path});
}

//--------------------------------------------------------------------------------------------------------------

void MeshManager::RegisterSkinMesh(SkinMeshList SkinMeshID, LPCTSTR path)
{
	m_SkinMeshInfoList.push_back({ SkinMeshID, path });
}

