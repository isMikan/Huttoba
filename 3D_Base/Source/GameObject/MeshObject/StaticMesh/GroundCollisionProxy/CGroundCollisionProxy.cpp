#include "CGroundCollisionProxy.h"

CGroundCollisionProxy::CGroundCollisionProxy(CGroundManager& groundManager)
	: m_pGroundManager	( groundManager )
	, m_GroundCount		()
	, m_OldGroundCount	()
{
	m_vPosition = D3DXVECTOR3(0.f, -1.f, 10.f);
}

CGroundCollisionProxy::~CGroundCollisionProxy()
{
	//“–‚½‚è”»’èíœ
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void CGroundCollisionProxy::Update()
{
	m_GroundCount = m_pGroundManager.GetGrounds().size();

	if (m_GroundCount != m_OldGroundCount)
	{
		switch (m_GroundCount)
		{
		case 0:

		case 1:
			//“–‚½‚è”»’èíœ
			CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

			//“–‚½‚è”»’è‚Ìì¬
			m_pCollision = CollisionDataFactory::CreateSphereForMesh(
				CollisionBase::ColliderTag::Ground,
				AssetManager::Mesh(StaticMeshList::floor1),
				this
			);
			break;
		case 2:
			//“–‚½‚è”»’èíœ
			CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

			//“–‚½‚è”»’è‚Ìì¬
			m_pCollision = CollisionDataFactory::CreateSphereForMesh(
				CollisionBase::ColliderTag::Ground,
				AssetManager::Mesh(StaticMeshList::floor2),
				this
			);
			break;
		case 3:
			//“–‚½‚è”»’èíœ
			CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

			//“–‚½‚è”»’è‚Ìì¬
			m_pCollision = CollisionDataFactory::CreateSphereForMesh(
				CollisionBase::ColliderTag::Ground,
				AssetManager::Mesh(StaticMeshList::floor3),
				this
			);
			break;
		case 4:
			//“–‚½‚è”»’èíœ
			CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());

			//“–‚½‚è”»’è‚Ìì¬
			m_pCollision = CollisionDataFactory::CreateSphereForMesh(
				CollisionBase::ColliderTag::Ground,
				AssetManager::Mesh(StaticMeshList::floor4),
				this
			);
			break;
		default:
			break;
		}
	}

	m_OldGroundCount = m_GroundCount;
}

void CGroundCollisionProxy::OnCollision(CollisionBase* other)
{
}
