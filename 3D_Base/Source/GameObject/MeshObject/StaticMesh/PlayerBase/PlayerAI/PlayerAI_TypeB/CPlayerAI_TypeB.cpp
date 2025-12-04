#include "CPlayerAI_TypeB.h"

CPlayerAI_TypeB::CPlayerAI_TypeB(int index)
	: CPlayerAI			( index )

	, m_DistanceWeight	( 0.5f )	//値を変えるとアイテム距離スコアが変化
									//値を大きくすると近くのアイテム、小さくすると好みのアイテムを優先する
{
}

CPlayerAI_TypeB::~CPlayerAI_TypeB()
{
}

void CPlayerAI_TypeB::Update()
{
	m_Control = ActionInstruct::None;

	SearchItem();

	CPlayerAI::Update();
}

void CPlayerAI_TypeB::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerAI::Draw(View, Proj, Light, Camera);
}

void CPlayerAI_TypeB::SearchItem()
{
	//アイテムを持っているなら戻す
	if (m_pItemBase) return;

	//狙うアイテム候補
	ItemBase* targetItem = nullptr;

	//最も高いスコアを記録
	float maxScore = -100000;

	for (auto& item : m_pItemManager->GetItems())
	{
		//アイテムが地面に落ちていないなら無視する
		//近くのアイテムが降ってくるまで待つならこのif分を無効化
		if (item->GetState() != ItemBase::State::OnGround) continue;

		//アイテムとの距離の計算
		D3DXVECTOR3 distance = item->GetPosition() - m_vPosition;

		//処理負荷軽減のために距離の2乗の値を渡す
		float distanceSq = D3DXVec3LengthSq(&distance);

		//スコアは距離が近いほうが高くしたいので
		float score = -distanceSq * m_DistanceWeight;

		//スコアが今までの最大より大きいなら
		if (maxScore < score)
		{
			maxScore = score;

			//ターゲットを更新
			targetItem = item.get();
			// 移動用の方向ベクトルも保存
			//D3DXVec3Normalize(&m_NearbyItems.dir, &distance);
			m_NearbyItems.dir = distance;
			m_NearbyItems.sqrt = distanceSq;
		}
	}

	//ターゲットが決まったら移動指示
	if (targetItem)
	{
		AutomaticMovement(m_NearbyItems.dir);
		//アイテムが十分近ければ拾う指示
		if (m_NearbyItems.sqrt < 1.0f)
		{
			m_Control = ActionInstruct::ToggleItem;
		}
	}
}
