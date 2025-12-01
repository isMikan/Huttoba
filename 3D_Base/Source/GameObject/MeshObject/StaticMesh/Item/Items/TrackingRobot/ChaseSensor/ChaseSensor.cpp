#include "ChaseSensor.h"
#include "PlayerBase/CPlayerBase.h"
#include "GroundCollisionProxy/CGroundCollisionProxy.h"

ChaseSensor::ChaseSensor(D3DXVECTOR3 pos, D3DXVECTOR3 searchOffset)
	: m_pTarget				()
	, m_pIgnoredPlayer		()
	, m_pTargetList			()

	, m_pCollision			()

	, m_CollisionOffSet		()
{
	for (int i = 0;i < 3;i++)
	{
		m_pCollision[i] = CollisionDataFactory::CreateSphereForMesh(
			CollisionBase::ColliderTag::TRobotSeachCol,
			AssetManager::Mesh(StaticMeshList::ExplosionCol),
			this
		);
	}	

	D3DXVECTOR3 Offset = searchOffset;

	m_pCollision[0]->SetLocalOffset(Offset);

	Offset.x = -1;

	Offset.z += (Offset.z / 2);

	//当たり判定の補正値を追加
	m_pCollision[1]->SetLocalOffset(Offset);


	Offset.x = 1;

	m_pCollision[2]->SetLocalOffset(Offset);
}

ChaseSensor::~ChaseSensor()
{
	for (int i = 0;i < m_pCollision.size();i++)
	{
		CollisionManager::GetInstance()->RemoveCollider(m_pCollision[i].get());
	}
}

void ChaseSensor::Update()
{
	FindNearestTarget();
}

void ChaseSensor::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			if (m_pIgnoredPlayer != player)
			{
				//当たったプレイヤーを記憶
				m_pTargetList.push_back(player);
			}
		}
	}
}

void ChaseSensor::FindNearestTarget()
{
	//配列の中身が空かどうかを判定
	if (m_pTargetList.empty())
	{
		//索敵範囲にプレイヤーがいない
		m_pTarget = nullptr;
	}
	else
	{
		m_pTarget = m_pTargetList[0];

		//ターゲット方向へのベクトルを計算
		D3DXVECTOR3 nowDirToTarget = m_pTarget->GetPosition() - m_vPosition;

		//ベクトルの長さを計算
		float nowVecLen = D3DXVec3LengthSq(&nowDirToTarget);

		//一番近いプレイヤーのポインタをとる
		CPlayerBase* pClosestTarget = m_pTargetList[0];

		for (size_t i = 1; i < m_pTargetList.size();i++)
		{
			//ターゲット方向へのベクトルを計算
			D3DXVECTOR3 nextDirToTarget = m_pTargetList[i]->GetPosition() - m_vPosition;

			//ベクトルの長さを計算
			float nextVecLen = D3DXVec3LengthSq(&nextDirToTarget);

			//現在のターゲットよりもターゲットリストの方が長さが短いなら
			if (nowVecLen > nextVecLen)
			{
				//長さを短いほうに更新
				nowVecLen = nextVecLen;

				//ターゲットを更新
				pClosestTarget = m_pTargetList[i];

			}
		}

		//近いプレイヤー検索し終わったので中身を破棄
		m_pTargetList.clear();

		//最後に残ったターゲットを入れる
		m_pTarget = pClosestTarget;
	}
}