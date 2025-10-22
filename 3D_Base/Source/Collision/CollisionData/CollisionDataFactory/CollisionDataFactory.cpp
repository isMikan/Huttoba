#include "stdafx.h"
#include "CollisionDataFactory.h"

HRESULT CollisionDataFactory::CreateSphereDataFromMesh(const std::shared_ptr<CStaticMesh> pMesh, CollisionSphere* outSphere)
{
	if (!pMesh || !outSphere) return E_FAIL; // Nullチェック

	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	// 頂点バッファ
	void* pVertices = nullptr;				// 頂点データへのポインタ
	D3DXVECTOR3 Center(0.f, 0.f, 0.f);		// 計算された中心座標
	float Radius = 0.f;						// 計算された半径

	// 頂点バッファを取得
	if (FAILED(pMesh->GetMesh()->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	// メッシュの頂点バッファをロックする
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	// メッシュの外接円の中心と半径を計算する（ロジックの本体）
	D3DXComputeBoundingSphere(
		static_cast<const D3DXVECTOR3*>(pVertices),		// 頂点データ
		pMesh->GetMesh()->GetNumVertices(),				// 頂点の数
		D3DXGetFVFVertexSize(pMesh->GetMesh()->GetFVF()), // 頂点サイズ
		&Center,										// (out) 中心座標
		&Radius);										// (out) 半径

	// メッシュの頂点バッファをアンロックし、解放する
	if (pVB != nullptr) {
		pVB->Unlock();
		SAFE_RELEASE(pVB);
	}

	// 計算結果を CollisionSphere データクラスに設定する
	//    メッシュのローカル座標の中心を、親オブジェクトからのオフセットとして設定
	outSphere->SetLocalOffset(Center);
	outSphere->SetRadius(Radius);

	return S_OK;
}
