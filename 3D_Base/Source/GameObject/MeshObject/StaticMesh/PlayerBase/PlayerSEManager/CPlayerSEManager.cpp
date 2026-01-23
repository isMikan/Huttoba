#include "CPlayerSEManager.h"

#include "Sound/CSoundManager.h"

CPlayerSEManager::CPlayerSEManager(std::unique_ptr<CPlayerManager>& playerManager)
	: m_pPlayerManager			( playerManager )

	, m_IsSE					( false )
{
	m_IsSE.resize(PlayerSEList::Max);
	SetPlayerSE();
}

CPlayerSEManager::~CPlayerSEManager()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 効果音の設定 ---.
void CPlayerSEManager::SetPlayerSE()
{
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player.get()) continue;	//存在しなかったら次へ.

		auto& bus = player.get()->GetBus();
		//購買処理.
		bus.Subscribe([this, &player](CPlayerState* state) {
			//拾う状態の場合.
			if (dynamic_cast<CPlayerPickupState*>(state))
			{
				if (!m_IsSE[Pickup])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_Pickup);
					m_IsSE[Pickup] = true;
				}
			}
			else
			{
				if (m_IsSE[Pickup]) m_IsSE[Pickup] = false;
			}
			//手の攻撃か捨てる状態の場合.
			if (dynamic_cast<CPlayerHandAttackState*>(state)
				|| dynamic_cast<CPlayerThrowState*>(state))
			{
				if (!m_IsSE[AttackHand])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_AttackHand);
					m_IsSE[AttackHand] = true;
				}
			}
			else
			{
				if (m_IsSE[AttackHand]) m_IsSE[AttackHand] = false;
			}
			//手の攻撃空振り状態の場合.
			if (dynamic_cast<CPlayerHandAttackState*>(state))
			{
				if (!m_IsSE[Whiff])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_Whiff);
					m_IsSE[Whiff] = true;
				}
			}
			else
			{
				if (m_IsSE[Whiff]) m_IsSE[Whiff] = false;
			}
			//押し出し状態の場合.
			if (dynamic_cast<CPlayerPushedState*>(state))
			{
				if (!m_IsSE[Pushed])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_HitHand);
					m_IsSE[Pushed] = true;
				}
			}
			else
			{
				if (m_IsSE[Pushed]) m_IsSE[Pushed] = false;
			}
			//吹き飛ばし状態の場合.
			if (dynamic_cast<CPlayerKnockbackState*>(state))
			{
				if (!m_IsSE[Knockback])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_HitHaetataki);
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_Knockback);
					m_IsSE[Knockback] = true;
				}
			}
			else
			{
				if (m_IsSE[Knockback]) m_IsSE[Knockback] = false;
			}
			//ダウン状態の場合.
			if (dynamic_cast<CPlayerKnockdownState*>(state))
			{
				if (!m_IsSE[Knockdown])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlayLoop(enSoundList::SE_Knockdown);
					m_IsSE[Knockdown] = true;
				}
			}
			else
			{
				if (m_IsSE[Knockdown])
				{
					//SEを止める.
					AssetManager::Sound()->Stop(enSoundList::SE_Knockdown);
					m_IsSE[Knockdown] = false;
				}
			}
			//落ちる状態の場合（落ち終わったときに鳴らす）.
			if (dynamic_cast<CPlayerFallingState*>(state))
			{
				m_IsSE[Falling] = true;
			}
			else
			{
				if (m_IsSE[Falling])
				{
					//SEを鳴らす.
					AssetManager::Sound()->PlaySE(enSoundList::SE_Down);
					m_IsSE[Falling] = false;
				}
			}
		});
	}
}

//--- 更新関数 ---.
void CPlayerSEManager::Update()
{
}