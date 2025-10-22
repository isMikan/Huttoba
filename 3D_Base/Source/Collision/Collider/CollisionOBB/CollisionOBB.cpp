//#include "stdafx.h"
//#include "CollisionOBB.h"
//
//CollisionObb::CollisionObb()
//	: m_Size		{1.f,1.f,1.f}
//	, m_WorldAxes{}
//	, m_World		{}
//{
//	// 初期化
//	D3DXMatrixIdentity(&m_World);
//	m_WorldAxes[0] = D3DXVECTOR3(1, 0, 0);
//	m_WorldAxes[1] = D3DXVECTOR3(0, 1, 0);
//	m_WorldAxes[2] = D3DXVECTOR3(0, 0, 1);
//}
//
//CollisionObb::~CollisionObb()
//{
//}
//
//
//void CollisionObb::UpdateWorldMat()
//{
//	//スケール・回転・移動の行列作成
//	D3DXMATRIX matS, matR, matT;
//	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
//	D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
//	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//
//	//ワールド行列を計算
//	m_World = matS * matR * matT;
//
//	//ローカル軸を更新
//	m_WorldAxes[0] = D3DXVECTOR3(m_World._11, m_World._21, m_World._31); // X軸
//	m_WorldAxes[1] = D3DXVECTOR3(m_World._12, m_World._22, m_World._32); // Y軸
//	m_WorldAxes[2] = D3DXVECTOR3(m_World._13, m_World._23, m_World._33); // Z軸
//
//	// ワールド半サイズの計算
//	// OBBのExtentsはローカルサイズにスケールを乗じた値になる
//	D3DXVECTOR3 localHalfSize = m_Size * 0.5f;
//
//	//解説メモ
//
//	// X, Y, Z軸それぞれのワールド方向へのスケーリング後の長さを計算
//	// OBBのワールド半サイズ（h_x, h_y, h_z）は、ローカル半サイズに、
//	// m_World (スケーリング・回転・移動) のスケーリング要素を適用した値。
//	// 単純化のため、ここではローカルサイズにローカルスケールを適用します。
//
//	// 厳密な計算（各軸の長さがワールド行列に含まれるスケーリングでどう伸びたか）
//	// Extentsは、ローカル半サイズ * ワールド行列の対応する軸の長さで計算するのが厳密
//	// D3DXVECTOR3 scale;
//	// D3DXQuaternion rotation;
//	// D3DXVECTOR3 translation;
//	// D3DXMatrixDecompose(&scale, &rotation, &translation, &m_World);
//
//	// m_Extents[0] = localHalfSize.x * scale.x;
//	// m_Extents[1] = localHalfSize.y * scale.y;
//	// m_Extents[2] = localHalfSize.z * scale.z;
//
//	// m_vScaleがローカルスケールとして使われているため、
//	// 外部からローカルサイズとローカルスケールを取得できる構造にするか、
//	// 軸ベクトルの長さ（正規化されているため1.0）を使うかを決める必要があります。
//	// 
//
//	// 正規化（数値誤差で軸が伸びるのを防ぐ）
//	for (int i = 0; i < 3; ++i)
//		D3DXVec3Normalize(&m_WorldAxes[i], &m_WorldAxes[i]);
//}
//
//D3DXVECTOR3 CollisionObb::GetHalfSizeScaled() const
//{
//	// 要素ごとに計算
//	return D3DXVECTOR3(
//		m_Size.x * m_vScale.x * 0.5f,
//		m_Size.y * m_vScale.y * 0.5f,
//		m_Size.z * m_vScale.z * 0.5f
//	);
//}