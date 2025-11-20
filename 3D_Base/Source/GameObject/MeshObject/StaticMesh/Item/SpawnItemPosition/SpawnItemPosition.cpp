#include "stdafx.h"
#include "SpawnItemPosition.h"
#include <algorithm>


SpawnItemPosition::SpawnItemPosition(std::unique_ptr<CGroundManager>& GroundMamager)
	: m_pGroundManager	{ GroundMamager }
	, m_CurrentFallGround{}
	, m_CurrentClampRangeMax	{}
	, m_CurrentClampRangeMin	{}
{
	Init();
}

SpawnItemPosition::~SpawnItemPosition()
{
}

//--------------------------------------------------------------------------------------------------------------

void SpawnItemPosition::Init()
{
	m_CurrentFallGround = GroundTag::None;
}

//--------------------------------------------------------------------------------------------------------------

void SpawnItemPosition::Uptate()
{
	//ステージがすでに落ちているか判定
	for (auto& ground : m_pGroundManager->GetGrounds())
	{
		//ステージが落ちているか
		if (ground->GetIsChangeColor())
		{
			m_CurrentFallGround = ground->GetTag();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------

//=================================================
//		位置決定
//=================================================
D3DXVECTOR3 SpawnItemPosition::SerectPosition()
{
	//現在のスポーン範囲を設定
	CheckCurrentGround();

	//現在のスポーン範囲からランダムに位置を設定
	VECTOR2 pos = SetRandomPos();
	D3DXVECTOR3 returnPos = { pos.x ,15.f,pos.z };

	return returnPos;
}

//以下機能
//--------------------------------------------------------------------------------------------------------------

void SpawnItemPosition::CheckCurrentGround()
{
	//各ステージ状況のスポーン範囲
	static const VECTOR2 SAFE_GROUND_RANGE_MAX = { 3.7f,13.6f };
	static const VECTOR2 SAFE_GROUND_RANGE_MIN = { -3.7f,6.3f };

	static const VECTOR2 THIRD_GROUND_RANGE_MAX = { 5.4f,15.5f };
	static const VECTOR2 THIRD_GROUND_RANGE_MIN = { -5.4f,4.3f };

	static const VECTOR2 SECOND_GROUND_RANGE_MAX = { 6.6f,17.9f };
	static const VECTOR2 SECOND_GROUND_RANGE_MIN = { -6.6f,2.3f };

	static const VECTOR2 FIRST_GROUND_RANGE_MAX = { 8.9f,19.1f };
	static const VECTOR2 FIRST_GROUND_RANGE_MIN = { -8.9f,0.75f };

	//現在のスポーン範囲を設定
	switch (m_CurrentFallGround)
	{
	case GroundTag::SafeGround:
		m_CurrentClampRangeMax = SAFE_GROUND_RANGE_MAX;
		m_CurrentClampRangeMin = SAFE_GROUND_RANGE_MIN;
		std::cout << "SafeGround" << std::endl;

		break;
	case GroundTag::ThirdFallGround:
		m_CurrentClampRangeMax = SAFE_GROUND_RANGE_MAX;
		m_CurrentClampRangeMin = SAFE_GROUND_RANGE_MIN;
		std::cout << "SafeFallGround" << std::endl;

		break;
	case GroundTag::SecondFallGround:
		m_CurrentClampRangeMax = THIRD_GROUND_RANGE_MAX;
		m_CurrentClampRangeMin = THIRD_GROUND_RANGE_MIN;
		std::cout << "ThirdRange" << std::endl;

		break;
	case GroundTag::FirstFallGround:
		m_CurrentClampRangeMax = SECOND_GROUND_RANGE_MAX;
		m_CurrentClampRangeMin = SECOND_GROUND_RANGE_MIN;
		std::cout << "SecondRange" << std::endl;

		break;
	case GroundTag::None:
		m_CurrentClampRangeMax = FIRST_GROUND_RANGE_MAX;
		m_CurrentClampRangeMin = FIRST_GROUND_RANGE_MIN;
		std::cout << "FirstRange" << std::endl;

		break;
	}
}

//--------------------------------------------------------------------------------------------------------------

VECTOR2 SpawnItemPosition::SetRandomPos()
{
	//ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> RandomPosX(m_CurrentClampRangeMin.x, m_CurrentClampRangeMax.x);
	std::uniform_real_distribution<float> RandomPosZ(m_CurrentClampRangeMin.z, m_CurrentClampRangeMax.z);

	return VECTOR2(RandomPosX(gen), RandomPosZ(gen));
}

