#include "CPlayerManager.h"

CPlayerManager::CPlayerManager(int index)
	: m_pPlayer		()
	, m_PlayerID	(index)
{
	Create();
}

CPlayerManager::~CPlayerManager()
{
}

//構築関数.
HRESULT CPlayerManager::Create()
{
	return S_OK;
}

//データの読み込み関数.
HRESULT CPlayerManager::LoadData()
{
	return S_OK;
}

//初期化関数.
void CPlayerManager::Init()
{
	for (auto& player : m_pPlayer)
	{
		player.SetObjectColor(SetCharacterColor(m_PlayerID));
		player.SetPosition(SetDefaultPosition(m_PlayerID))
	}
}

//破棄関数.
void CPlayerManager::Destroy()
{
}

//更新関数.
void CPlayerManager::Update()
{
}

//描画関数.
void CPlayerManager::Draw()
{
}

//キャラクターの色を設定する関数.
CPlayer::ObjectColor CPlayerManager::SetCharacterColor(int index)
{
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor;	//プレイヤーの色.
	switch (index)
	{
	case 0:
		playerColor[index] = {
			D3DXVECTOR4(1.f, 0.f, 0.f, 1.f), // 赤
			D3DXVECTOR4(0.5f, 0.f, 0.f, 1.f), // 少し暗めの赤
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),
		};

		break;
	case 1:
		playerColor[index] = {
			D3DXVECTOR4(0.f, 0.f, 1.f, 1.f),  // 青
			D3DXVECTOR4(0.f, 0.f, 0.5f, 1.f),  // 少し暗めの青
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 2:
		playerColor[index] = {
			D3DXVECTOR4(1.0f, 0.5f, 0.f, 1.f),  // オレンジ
			D3DXVECTOR4(0.5f, 0.3f, 0.f, 1.f), // 少し暗めのオレンジ
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 3:
		playerColor[index] = {
			D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.f),  // 緑
			D3DXVECTOR4(0.0f, 0.5f, 0.0f, 1.f),  // 少し暗めの緑
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	default:
		break;
	}
	return playerColor[index];
}

//初期位置.
D3DXVECTOR3 CPlayerManager::SetDefaultPosition(int index)
{
	std::array<D3DXVECTOR3, Player_Max> playerPos;	//プレイヤーの位置.

	switch (index)
	{
	case 0:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 5.f);

		break;
	case 1:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 5.f);

		break;
	case 2:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 10.f);

		break;
	case 3:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 10.f);

		break;
	default:
		break;
	}

	return playerPos[index];
}
