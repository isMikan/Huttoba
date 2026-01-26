#include "CPlayerAI_TypeB.h"
#include <PlayerBase/PlayerState/PlayerActionState/PlayerKnockbackState/CPlayerKnockbackState.h>
#include <PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h>

#include "Ground/GroundManager/CGroundManager.h"

CPlayerAI_TypeB::CPlayerAI_TypeB(int index)
	: CPlayerAI				( index )

	, m_Destination			()
	, m_MoveScore			()

	, m_DistanceWeight		( 0.5f )	//値を変えるとアイテム距離スコアが変化
										//値を大きくすると近くのアイテム、小さくすると好みのアイテムを優先する

	, m_GroundSafeRadius	( 0.8f )	//値を変えるとステージのこれより外側は危険の範囲が変化 0~1の値を入れて
{
}

CPlayerAI_TypeB::~CPlayerAI_TypeB()
{
}

void CPlayerAI_TypeB::Update()
{
	m_Control = ActionInstruct::None;

	m_Destination.dir = { 0.f,0.f,0.f };

	//毎フレーム行動スコアを低くする
	//0にすると scoer = -距離 をしているので何も行動しないのスコアが高くなる
	m_MoveScore = -10000;

	SearchItem();

	HandleItemAction();

	RunBomb();

	m_Destination.dir += CalculateStage();

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
		&& m_pItemManager->GetItemVectorNum() > 0)	//アイテムの数が0より多いなら
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

			//アイテムの種類でスコアの増減を行う
			score += ItemScoreBonus(item->GetTag());

			//スコアが今までの最大より大きいなら
			if (m_MoveScore < score)
			{
				m_MoveScore = score;

				//ターゲットを更新
				targetItem = item.get();

				D3DXVec3Normalize(&distance, &distance);

				m_Destination.dir = distance;
				m_Destination.sqrt = distanceSq;
			}
		}
	}

	//ターゲットが決まったら移動
	if (targetItem)
	{
		//アイテムが近ければ拾う
		if (m_Destination.sqrt < 2.0f)
		{
			m_Control = ActionInstruct::ToggleItem;
		}
	}
}

void CPlayerAI_TypeB::HandleItemAction()
{
	////アイテムを未所持なら返す
	//if (!m_pHoldingItem)return;

	//狙うプレイヤー
	CPlayerBase* targetPlayer = nullptr;

	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		//自分なら無視して次へ
		if (player.get() == this)continue;

		//中身がないなら無視して次へ
		if (!player)continue;

		//他プレイヤーとの距離の計算
		D3DXVECTOR3 distance = player->GetPosition() - m_vPosition;

		//処理負荷軽減のために距離の2乗の値を渡す
		float distanceSq = D3DXVec3LengthSq(&distance);

		//スコアは距離が近いほうが高くしたいので
		float score = -distanceSq * m_DistanceWeight;

		//相手プレイヤーがダウン中か吹き飛んでいる最中なら攻撃しにくくする
		if (player->IsAnyActionState<CPlayerKnockdownState,CPlayerKnockbackState>())
		{
			//score -= 5000.0f;
		}

		score += CalculateDangerScore(player->GetPosition());

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
		//正規化
		D3DXVECTOR3 nor = targetPlayer->GetPosition() - m_vPosition;
		D3DXVec3Normalize(&nor, &nor);

		//向いている方向
		D3DXVECTOR3 forward = GetLocalAxes().forward;

		//内積の計算
		//1で同じ方向,0で直角,-1で真後ろを向いている
		float dot = D3DXVec3Dot(&nor, &forward);


		if (m_pHoldingItem)
		{
			//ターゲットプレイヤーと近ければ攻撃
			if (m_Destination.sqrt < ItemMove(m_pHoldingItem->GetTag())/* && dot>0.5f*/)
			{
				m_Control = ActionInstruct::Attack;
			}
		}
		else
		{
			//ターゲットプレイヤーと近ければ攻撃
			if (m_Destination.sqrt < 1.0f /*&& dot>0.5f*/)
			{
				m_Control = ActionInstruct::Attack;
			}
		}
		std::cout << targetPlayer->GetPlayerID() << std::endl;
	}
}

float CPlayerAI_TypeB::ItemMove(ItemID item)
{
	//アイテムによって変化する攻撃するの距離
	float AttackDistance;

	switch (item)
	{
	case ItemID::Haetataki:
		AttackDistance = 1.0f;
		break;
	case ItemID::Bomb:
		AttackDistance = 4.0f;
		break;
	case ItemID::Fun:
		AttackDistance = 5.0f;
		break;
	case ItemID::Mushroom:
		AttackDistance = 4.0f;
		break;
	case ItemID::SmashBat:
		AttackDistance = 1.0f;
		break;
	case ItemID::TrackingRobot:
		AttackDistance = 10.0f;
		break;
	case ItemID::Boomerang:
		AttackDistance = 1.0f;
		break;
	case ItemID::Magnet:
		AttackDistance = 1.0f;
		break;
	case ItemID::Max:
		//該当しないものなら0を入れて攻撃しないようにする
		AttackDistance = 1.0f;
		break;
	case ItemID::None:
		//該当しないものなら0を入れて攻撃しないようにする
		AttackDistance = 1.0f;
		break;
	default:
		//該当しないものなら0を入れて攻撃しないようにする
		AttackDistance = 0.0f;
		break;
	}
	return AttackDistance;
}

float CPlayerAI_TypeB::ItemScoreBonus(const ItemID& item) const
{
	switch (item)
	{
	case ItemID::Haetataki:
		return 10.f;
		break;
	case ItemID::Bomb:
		return 10.f;
		break;
	case ItemID::Fun:
		return 10.f;
		break;
	case ItemID::Mushroom:
		return 10.f;
		break;
	case ItemID::SmashBat:
		return 10.f;
		break;
	case ItemID::TrackingRobot:
		return 10.f;
		break;
	case ItemID::Boomerang:
		return 10.f;
		break;
	case ItemID::Magnet:
		return 10.f;
		break;
	case ItemID::Max:
		break;
	case ItemID::None:
		break;
	default:
		break;
	}

	return 0.0f;
}

D3DXVECTOR3 CPlayerAI_TypeB::CalculateStage()
{
	//地面の中心位置をとる
	D3DXVECTOR3 groundCenter = m_pGroundManager->GetGroundCenterPos();

	//地面の半径の全長を計算
	float groundRadius = m_pGroundManager->GetGroundRadius();

	//地面の安全範囲
	float safeRadius = groundRadius * m_GroundSafeRadius;

	//安全範囲の2乗した値
	float safeRadiusSq = safeRadius * safeRadius;

	//中心位置からプレイヤーの位置へのベクトル
	D3DXVECTOR3 diff = m_vPosition - groundCenter;

	//中心からプレイヤーへのベクトルの長さ(y座標を入れていないのはプレイヤーの高さに関係なくステージ外に移動してほしくないから)
	float distSq = diff.x * diff.x + diff.z * diff.z;

	//ベクトルの長さが安全圏の範囲よりも外側に出たら
	if (safeRadiusSq < distSq)
	{
		//中心に向かうベクトル
		D3DXVECTOR3 centerVec = groundCenter - m_vPosition;
		
		D3DXVec3Normalize(&centerVec, &centerVec);

		//中心からプレイヤーへのベクトルをルート計算
		float dist = sqrtf(distSq);
		//中心からプレイヤーへのベクトルが安全範囲をどれくらい超えているかを計算
		float overDist = dist - safeRadius;

		//超えている長さによってより強い力で中央に戻るように
		float weight = overDist * 15.0f;

		//中心に向かうベクトルにどれくらいの強さで中央にに戻るかを返す
		return centerVec * weight;
	}

	//安全範囲内なので全て0を返す
	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

float CPlayerAI_TypeB::CalculateDangerScore(const D3DXVECTOR3& pos) const
{
	//地面の中心位置をとる
	D3DXVECTOR3 groundCenterPos = m_pGroundManager->GetGroundCenterPos();

	//中心位置から引数の位置がどれくらい離れているかを計算
	D3DXVECTOR3 dist = pos - groundCenterPos;

	//先ほど出した中心から離れているベクトルの長さから全長を消す(y座標がないのはy軸移動がほぼないから)
	float distSq = dist.x * dist.x + dist.z * dist.z;

	//地面の安全とする範囲を計算
	float groundSafeRadius = m_pGroundManager->GetGroundRadius() * m_GroundSafeRadius;

	//distSqがルートの計算を省いているので2乗する
	float groundSafeRadiusSq = groundSafeRadius * groundSafeRadius;

	//距離が地面の安全とする範囲より下なら
	if (distSq < groundSafeRadiusSq)
	{
		return 0;
	}

	float groundRadiusSq = m_pGroundManager->GetGroundRadius() * m_pGroundManager->GetGroundRadius();

	//引数の位置-安全なステージ範囲/ステージの全長-安全な範囲 をして割合を求める
	float danger = (distSq - groundSafeRadiusSq) / (groundRadiusSq - groundSafeRadiusSq);

	//1.0以上ならステージ外に出ているので必ず無視するくらい点数を低くする
	if (danger >= 1.0f)
		return -(100 * 100);

	//0~1で10をかけて点数を引くくする
	return -10 * danger;
}

void CPlayerAI_TypeB::RunBomb()
{
	for (auto& item : m_pItemManager->GetItems())
	{
		if (item->GetTag() == ItemID::Bomb)
		{
			if (item->GetState() == IItemObserver::State::Throw|| item->GetState() == IItemObserver::State::Use)
			{
				m_Destination.dir += m_vPosition - item->GetPosition();
			}
		}
	}
}

D3DXVECTOR3 CPlayerAI_TypeB::CalculateAvoidDanger()
{
	for (auto& item : m_pItemManager->GetItems())
	{

		D3DXVECTOR3 itemPos = item->GetPosition();

		float itemLenSq = D3DXVec3LengthSq(&itemPos);

		if (item->GetState() == IItemObserver::State::Throw)
		{

		}

		if (item->GetTag() == ItemID::Bomb)
		{
			if (item->GetState() == IItemObserver::State::Use)
			{
				m_Destination.dir += m_vPosition - item->GetPosition();
			}
		}

		if (item->GetTag() == ItemID::Boomerang)
		{

		}
	}
	return D3DXVECTOR3();
}
