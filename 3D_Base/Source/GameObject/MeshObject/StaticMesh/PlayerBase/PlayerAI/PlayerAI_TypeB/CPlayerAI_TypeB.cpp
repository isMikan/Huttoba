#include "CPlayerAI_TypeB.h"
#include <PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h>
#include <PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h>

#include "Ground/GroundManager/CGroundManager.h"

CPlayerAI_TypeB::CPlayerAI_TypeB(int index)
	: CPlayerAI			( index )

	, m_Destination		()
	, m_MoveScore		()

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

	//毎フレーム行動スコアを低くする
	//0にすると scoer = -距離 をしているので何も行動しないのスコアが高くなる
	m_MoveScore = -10000;

	SearchItem();

	HandleItemAction();

	AvoidDanger();

	AutomaticMovement(m_Destination.dir);

	CPlayerAI::Update();
}

void CPlayerAI_TypeB::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerAI::Draw(View, Proj, Light, Camera);
}

void CPlayerAI_TypeB::SearchItem()
{
	//アイテムを持っているなら返す
	if (m_pHoldingItem) return;

	//狙うアイテム
	ItemBase* targetItem = nullptr;

	if (m_pItemManager
		&& m_pItemManager->GetItemVectorNum() > 0)
	{
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


			switch (item->GetTag())
			{
			case ItemID::Haetataki:
				//score += 30;
				break;
			case ItemID::Bomb:
				break;
			case ItemID::Fun:
				break;
			case ItemID::Mushroom:
				break;
			case ItemID::SmashBat:
				break;
			case ItemID::TrackingRobot:
				break;
			case ItemID::Boomerang:
				break;
			case ItemID::None:
				break;
			default:
				break;
			}

			//スコアが今までの最大より大きいなら
			if (m_MoveScore < score)
			{
				m_MoveScore = score;

				//ターゲットを更新
				targetItem = item.get();

				m_Destination.dir = distance;
				m_Destination.sqrt = distanceSq;
			}
		}
	}

	//ターゲットが決まったら移動
	if (targetItem)
	{
		//アイテムが近ければ拾う
		if (m_Destination.sqrt < 3.0f)
		{
			m_Control = ActionInstruct::ToggleItem;
		}
	}
}

void CPlayerAI_TypeB::HandleItemAction()
{
	//アイテムを未所持なら返す
	if (!m_pHoldingItem)return;

	//狙うプレイヤー
	CPlayerBase* targetPlayer = nullptr;

	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		//自分なら無視して次へ
		if (player.get() == this)continue;

		//中身がないなら無視して次へ
		if (!player)continue;

		//ここでエラー
		//他プレイヤーとの距離の計算
		D3DXVECTOR3 distance = player->GetPosition() - m_vPosition;

		//処理負荷軽減のために距離の2乗の値を渡す
		float distanceSq = D3DXVec3LengthSq(&distance);

		//スコアは距離が近いほうが高くしたいので
		float score = -distanceSq * m_DistanceWeight;

		//相手プレイヤーがダウン中か吹き飛んでいる最中なら攻撃しにくくする
		if (player->IsAnyActionState<CPlayerKnockdownState,CPlayerKnockbackState>())
		{
			score -= 5000.0f;
		}

		//スコアが今までの最大より大きいなら
		if (m_MoveScore < score)
		{
			m_MoveScore = score;

			//ターゲットを更新
			targetPlayer = player.get();

			m_Destination.dir = distance;
			m_Destination.sqrt = distanceSq;
		}
	}

	//ターゲットが決まったら移動指示
	if (targetPlayer)
	{
		//ターゲットプレイヤーと近ければ攻撃
		if (m_Destination.sqrt < 1.0f)
		{
			m_Control = ActionInstruct::Attack;
		}
	}
}

void CPlayerAI_TypeB::AvoidDanger()
{
	//地面の中心位置をとる
	D3DXVECTOR3 groundCenterPos = m_pGroundManager->GetGroundCenterPos();

	//地面の半径の全長を計算
	float groundRadius = m_pGroundManager->GetGroundRadius() * m_pGroundManager->GetGroundRadius();

	//中心位置からプレイヤーの位置がどれくらい離れているかを計算
	D3DXVECTOR3 diff = m_vPosition - groundCenterPos;

	//中心位置からプレイヤーの離れているかの全長を出す
	float diffSq = diff.x * diff.x + diff.z * diff.z;

	std::cout << diffSq << std::endl;
	std::cout << groundRadius << std::endl;

	//プレイヤーの位置が地面の半径以上なら
	if (diffSq > groundRadius * (0.7f * 0.7f))
	{
		//一旦中央に移動
		m_Destination.dir = m_pGroundManager->GetGroundCenterPos() - m_vPosition;
	}
}