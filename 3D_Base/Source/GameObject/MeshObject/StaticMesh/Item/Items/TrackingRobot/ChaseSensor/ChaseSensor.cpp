#include "ChaseSensor.h"
#include "PlayerBase/CPlayerBase.h"

ChaseSensor::ChaseSensor(D3DXVECTOR3 pos, D3DXVECTOR3 searchOffset)
	: m_pTargetList		()
	, m_pTarget			()

	, m_pCollision		()

	, m_CollisionOffSet	()
	
	, m_IsHitGround		( false )
{
	m_pCollision = CollisionDataFactory::CreateSphereForMesh(
		CollisionBase::ColliderTag::TRobotSeachCol,
		AssetManager::Mesh(StaticMeshList::ExplosionCol),
		this
	);

	//当たり判定の補正値を追加
	m_pCollision->SetLocalOffset(searchOffset);
}

ChaseSensor::~ChaseSensor()
{
	CollisionManager::GetInstance()->RemoveCollider(m_pCollision.get());
}

void ChaseSensor::OnCollision(CollisionBase* other)
{
	if (other->GetTag() == CollisionBase::ColliderTag::Player)
	{
		if (CPlayerBase* player = dynamic_cast<CPlayerBase*>(other->GetListener()))
		{
			//当たったプレイヤーを記憶
			m_pTargetList.push_back(player);
		}
	}


	if (other->GetTag() == CollisionBase::ColliderTag::Ground)
	{
		m_IsHitGround = true;
	}
	else
	{
		m_IsHitGround = false;
		std::cout << "地面と当たってないよ" << std::endl;

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

		//最後に残ったターゲットを入れる
		m_pTarget = pClosestTarget;
	}
}
