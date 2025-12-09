#include "CGroundManager.h"

CGroundManager::CGroundManager()
	: m_pGrounds			()
{
	Create();
	//\
	m_FallTime = { 999.f, 999.f, 999.f, 999.f };
	m_FallTime = { 99.f, 45.f, 30.f, 10.f };

	ExtractMeshRadius();
}

CGroundManager::~CGroundManager()
{
	m_pGrounds.clear();
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 読込関数 ---.
void CGroundManager::LoadData()
{
	//地面のスタティックメッシュを設定.
	m_pGrounds[SafeGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SafeGround));
	m_pGrounds[SafeGround]->SetTag(GroundTag::SafeGround);

	m_pGrounds[ThirdFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::ThirdFallGround));
	m_pGrounds[ThirdFallGround]->SetTag(GroundTag::ThirdFallGround);

	m_pGrounds[SecondFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::SecondFallGround));
	m_pGrounds[SecondFallGround]->SetTag(GroundTag::SecondFallGround);

	m_pGrounds[FirstFallGround]->AttachMesh(AssetManager::Mesh(StaticMeshList::FirstFallGround));
	m_pGrounds[FirstFallGround]->SetTag(GroundTag::FirstFallGround);

}

//--- 更新関数 ---.
void CGroundManager::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	for (int gNo = 0; gNo < m_pGrounds.size(); gNo++)
	{
		//なかったら次へ.
		if (!m_pGrounds[gNo]) continue;

		//制限のところまできたら消す.
		if (m_pGrounds[gNo]->GetPosition().y < -30.f)
		{
			Destroy(gNo);
			return;
		}

		//落ちる時間が過ぎたら.
		if (m_FallTime[gNo] <= t)
		{
			if (!m_pGrounds[gNo]->GetIsFallDown())
			{
				m_pGrounds[gNo]->SetShakeTriggerTime(t);
				m_pGrounds[gNo]->SetIsChangeColor(false);
				m_pGrounds[gNo]->SetIsFallDown(true);
			}
		}
		//落ちる時間より(ChangeColorTime秒)前に落ちるまで.
		else if (m_FallTime[gNo] - CGround::m_ChangeColorTime <= t
			&& !m_pGrounds[gNo]->GetIsChangeColor())
		{
			m_pGrounds[gNo]->SetChangeColorTriggerTime(t);
			m_pGrounds[gNo]->SetIsChangeColor(true);
		}

		m_pGrounds[gNo]->Update();
	}
}

//--- 描画関数 ---.
void CGroundManager::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	for (auto& ground : m_pGrounds)
	{
		//なかったら次へ.
		if (!ground) continue;

		ground->Draw(View, Proj, Light, Camera);
	}
}

//--- メイン ---.
void CGroundManager::MainGroundCreate()
{
	Create();

	for (auto& ground : m_pGrounds)
	{
		//地面の位置を設定.
		ground->SetPosition(0.f, -1.f, 10.f);
	}
}

//--- リザルト ---.
void CGroundManager::ResultGroundCreate()
{
	Create();

	for (auto& ground : m_pGrounds)
	{
		//地面の位置を設定.
		ground->SetPosition(3.f, -1.f, -3.f);
	}
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

D3DXVECTOR3 CGroundManager::GetGroundCenterPos()
{
	D3DXVECTOR3 center = m_pGrounds[m_pGrounds.size() - 1]->GetPosition();

	center.x -= m_GroundRadius[m_pGrounds.size() - 1];
	center.z -= m_GroundRadius[m_pGrounds.size() - 1];

	return center;
}

//--- 構築関数 ---.
void CGroundManager::Create()
{
	m_pGrounds.clear();
	m_pGrounds.resize(Ground_Max);

	for (auto& ground : m_pGrounds)
	{
		//地面クラスのインスタンス作成.
		ground = std::make_unique<CGround>();
	}
}

//--- 破棄関数 ---.
void CGroundManager::Destroy(int index)
{
	m_pGrounds[index].reset();
	m_pGrounds.resize(m_pGrounds.size() - 1);
}

bool CGroundManager::ExtractMeshRadius()
{
	for (int i = 0;i < Ground_Max;i++)
	{
		if (!m_pGrounds[i] || !m_pGrounds[i]->GetMesh()) return false;

		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ
		void* pVertices = nullptr;				//頂点

		//グラウンドのメッシュ
		auto mesh = m_pGrounds[i]->GetMesh();

		//頂点バッファを取得
		if (FAILED(mesh->GetMesh()->GetVertexBuffer(&pVB))) return false;

		//メッシュの頂点バッファをロックする
		if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
		{
			SAFE_RELEASE(pVB);
			return false;
		}

		D3DXVECTOR3 outCenter;
		float		outRadius;

		//メッシュの外接円の中心と半径を計算する
		D3DXComputeBoundingSphere(
			static_cast<D3DXVECTOR3*>(pVertices),
			mesh->GetMesh()->GetNumVertices(),		//頂点の数
			mesh->GetMesh()->GetFVF(),				//頂点の情報
			&outCenter,								//(out)中心座標
			&outRadius);							//(out)半径

		//メッシュの頂点バッファをアンロックする
		if (pVB != nullptr)
		{
			pVB->Unlock();
			SAFE_RELEASE(pVB); // 取得したポインタを解放
		}

		m_GroundRadius[i] = outRadius;
	}

	return true;
}

