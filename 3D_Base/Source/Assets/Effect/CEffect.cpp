#include "CEffect.h"

//描画スプライト最大数
constexpr int RENDER_SPRITE_MAX = 8000;
//エフェクト管理用インスタンス最大数
constexpr int EFFECT_INSTANCE_MAX = 1000;

//DirectX Vector3 を Effekseer Vector3 に変換する
::EsVec3 CEffect::ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx)
{
	return ::EsVec3(pSrcVec3Dx->x, pSrcVec3Dx->y, pSrcVec3Dx->z);
}

//Effekseer Vector3 を DirectX Vector3 に変換する
D3DXVECTOR3 CEffect::ToDxVector3(const::EsVec3* pSrcVec3Efk)
{
	return D3DXVECTOR3(pSrcVec3Efk->X, pSrcVec3Efk->Y, pSrcVec3Efk->Z);
}

//DirectX Matrix を Effekseer Matrix に変換する
::EsMatrix CEffect::ToEfkMatrix(const D3DXMATRIX* pSrcMatDx)
{
	::EsMatrix OutMatEfk;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			OutMatEfk.Values[i][j] = pSrcMatDx->m[i][j];
		}
	}
	return OutMatEfk;
}

D3DXMATRIX CEffect::ToDxMatrix(const::EsMatrix* pSrcMatEfk)
{
	D3DXMATRIX OutMatDx;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			OutMatDx.m[i][j] = pSrcMatEfk->Values[i][j];
		}
	}
	return OutMatDx;
}

CEffect::CEffect()
	: m_pManager(nullptr)
	, m_pRenderer(nullptr)
{
}

//ビュー行列を設定
void CEffect::SetViewMatrix(const D3DXMATRIX& mView)
{
	::EsMatrix EsCamMat;	//カメラ行列
	EsCamMat = ToEfkMatrix(&mView);

	//カメラ行列を設定
	m_pRenderer->SetCameraMatrix(EsCamMat);
}

//プロジェクション行列を設定
void CEffect::SetProjectionMatrix(const D3DXMATRIX& mProj)
{
	::EsMatrix EsProjMat;	//プロジェクション行列
	EsProjMat = ToEfkMatrix(&mProj);

	//プロジェクション行列を設定
	m_pRenderer->SetProjectionMatrix(EsProjMat);
}

CEffect::~CEffect()
{
	//エフェクトのマネージャーを破棄
	m_pManager.Reset();

	//エフェクトのレンダラーを破棄
	m_pRenderer.Reset();
}

//構築
HRESULT CEffect::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
{
	//エフェクトのマネージャーの作成
	m_pManager = ::EsManager::Create(RENDER_SPRITE_MAX);

	//エフェクトのレンダラーの作成
	m_pRenderer = ::EsRenderer::Create(pDevice, pContext, EFFECT_INSTANCE_MAX);

	//描画モジュールの設定
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());

	//テクスチャ、モデル、マテリアルローダーの設定をする。
	//ユーザーが独自に拡張できる。現在はファイルから読み込んでいる。
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());
	m_pManager->SetMaterialLoader(m_pRenderer->CreateMaterialLoader());
	m_pManager->SetCurveLoader(::Es::MakeRefPtr<::Es::CurveLoader>());

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

//データ読み込み
HRESULT CEffect::LoadData()
{
	//キーとそれに対応するファイルを入れる
	m_pEffect["Laser"]  = ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Laser01.efk");
	m_pEffect["Laser1"] = ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Laser01.efk");
	m_pEffect["Laser2"] = ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Laser01.efk");

	m_pEffect["Explosion"]	=	::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Explosion.efk");
	m_pEffect["FunWind"]	=	::EsEffect::Create(m_pManager, u"Data\\Effekseer\\FunWind2.efk");
	m_pEffect["Break"]		=	::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Smoke.efk");

	m_pEffect["BoomerangMaxCharge"] = ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\BoomerangMaxCharge.efk");
	m_pEffect["BoomerangHit"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\BoomerangHit.efk");
	m_pEffect["SmashBatHave"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\SmashBatHave.efk");
	m_pEffect["SmashBatHit"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\SmashBatHit.efk");
	m_pEffect["HaetatakiHit"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\HaetatakiHit.efk");

	m_pEffect["mushroomarea"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\mushroomarea.efk");

	m_pEffect["Knockback"]		= ::EsEffect::Create(m_pManager, u"Data\\Effekseer\\Knockback.efk");

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

//描画
void CEffect::Draw(
	const D3DXMATRIX& mView, const D3DXMATRIX& mProj,
	const LIGHT& Light, const CAMERA& Camera)
{
	//ビュー行列を設定
	SetViewMatrix(mView);

	//プロジェクション行列を設定
	SetProjectionMatrix(mProj);

	//レイヤーパラメータ設定
	::EsManager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = ToEfkVector3(&Camera.vPosition);
	m_pManager->SetLayerParameter(0, layerParameter);

	//エフェクトの更新処理
	m_pManager->Update();

	//-------------------------
	// Effekseerレンダリング
	//-------------------------
	//エフェクトの描画開始処理を行う
	m_pRenderer->BeginRendering();

	//エフェクトの描画を行う
	m_pManager->Draw();

	//エフェクトの描画終了処理を行う
	m_pRenderer->EndRendering();
}
