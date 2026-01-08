#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh			( nullptr )

	, m_Billboard		( false )
{
	//初期色.
	ObjectColor color = {
		D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.f),
		D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.f),
		D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f)
	};
	//サイズを指定する.
	m_ObjectColor.resize(1);
	//色を設定.
	m_ObjectColor[0] = color;
}

CStaticMeshObject::~CStaticMeshObject()
{
	DetachMesh();
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if( m_pMesh == nullptr ){
		return;
	}

	//中身がないなら通らない.(突貫工事)
	if(m_vRotation != D3DXVECTOR3(0.f,0.f,0.f))
	{
		//オイラー角をクォータニオンに変換.
		D3DXQuaternionRotationYawPitchRoll(
			&m_vQuaternion, m_vRotation.x, m_vRotation.y, m_vRotation.z);
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetScale( m_vScale );
	m_pMesh->SetQuaternion( m_vQuaternion );

	for (int mNo = 0; mNo < m_ObjectColor.size(); mNo++)
	{
		//CStaticMeshに現在の色を渡す.
		m_pMesh->SetObjectColor(
			mNo, m_ObjectColor[mNo].diffuse, m_ObjectColor[mNo].ambient, m_ObjectColor[mNo].specular);
	}

	m_pMesh->SetBillboard(m_Billboard);

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}

std::pair<D3DXVECTOR3,float> CStaticMeshObject::GetCenterAndRadius()
{
	D3DXVECTOR3 center = { 0.f,0.f,0.f };
	float rad = 0.f;

	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	//頂点バッファ
	void* pVertices = nullptr;				//頂点

	//頂点バッファを取得
	m_pMesh->GetMesh()->GetVertexBuffer(&pVB);

	//メッシュの頂点バッファをロックする
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return { D3DXVECTOR3(0.f,0.f,0.f),0.f };
	}

	//メッシュの外接円の中心と半径を計算する
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>(pVertices),
		m_pMesh->GetMesh()->GetNumVertices(),					//頂点の数
		D3DXGetFVFVertexSize(m_pMesh->GetMesh()->GetFVF()),	//頂点の情報
		&center,											//(out)中心座標
		&rad);										//(out)半径

	//メッシュの頂点バッファをアンロックする
	if (pVB != nullptr)
	{
		pVB->Unlock();
		SAFE_RELEASE(pVB); // 取得したポインタを解放
	}

	return { center,rad };
}
