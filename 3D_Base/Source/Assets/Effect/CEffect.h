#pragma once

//警告についてのコード分析を無効にする。4005:再定義
#pragma warning(disable:4005)

//---------------------------------------
// Effekseer関係ヘッダ、ライブラリ
//---------------------------------------
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include "stdafx.h"

#include<vector>
#include<unordered_map>
#include<string>

#ifdef _DEBUG
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"EffekseerRendererDX11d.lib")
#else//#ifdef _DEBUG
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX11.lib")
#endif//#ifdef _DEBUG

//エイリアスを用意
//※コードが横に長くなって読みづらいため
namespace {
	namespace Es = ::Effekseer;
	using EsManagerRef = ::Es::ManagerRef;
	using EsManager = ::Es::Manager;
	using EsEffectRef = ::Es::EffectRef;
	using EsEffect = ::Es::Effect;
	using EsVec3 = ::Es::Vector3D;
	using EsMatrix = ::Es::Matrix44;
	using EsHandle = ::Es::Handle;
	using EsRendererRef = ::EffekseerRendererDX11::RendererRef;
	using EsRenderer = ::EffekseerRendererDX11::Renderer;
};

/**************************************************
*	フリーソフト Effekseerのデータを使うためのクラス
*	singleton(シングルトン：デザインパターンの１つ)で作成
**/
class CEffect
{
public:
	CEffect();
	~CEffect();

	//構築
	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//データ読み込み
	HRESULT LoadData();
	//描画
	void Draw(const D3DXMATRIX& mView, const D3DXMATRIX& mProj,
		const LIGHT& Light, const CAMERA& Camera);

	//-----------------------------------
	// 変換系
	//-----------------------------------
	//ベクター
	::EsVec3 ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx);
	D3DXVECTOR3 ToDxVector3(const ::EsVec3* pSrcVec3Efk);
	//行列
	::EsMatrix ToEfkMatrix(const D3DXMATRIX* pSrcMatDx);
	D3DXMATRIX ToDxMatrix(const ::EsMatrix* pSrcMatEfk);

	EsVec3 a;
	//-----------------------------------
	// 制御系
	//-----------------------------------
	//再生
	::EsHandle Play(std::string name, const D3DXVECTOR3& pos)
	{
		return m_pManager->Play(m_pEffect[name], pos.x, pos.y, pos.z);
	}

	/*
		停止
		すべて停止
		一時停止
		再生速度の設定
		位置を特定
		回転指定
		回転指定(軸回転)
		サイズ設定
	*/

	//停止
	void Stop(::EsHandle handle)
	{
		m_pManager->StopEffect(handle);
	}

	//すべて停止
	void StopAll()
	{
		m_pManager->StopAllEffects();
	}

	//一時停止
	void Stop(::EsHandle handle, bool paused)
	{
		m_pManager->SetPaused(handle, paused);
	}

	//再生速度の設定
	void SetSpeed(::EsHandle handle, float speed)
	{
		m_pManager->SetSpeed(handle, speed);
	}

	//位置を特定する
	void SetLocation(::EsHandle handle, D3DXVECTOR3 pos)
	{
		m_pManager->SetLocation(
			handle,
			::EsVec3(pos.x, pos.y, pos.z));
	}

	//回転を指定する
	void SetRotation(::EsHandle handle, D3DXVECTOR3 rot)
	{
		m_pManager->SetRotation(handle, rot.x, rot.y, rot.z);
	}

	//回転を指定する(軸回転)
	void SetRotation(::EsHandle handle, D3DXVECTOR3 vAxis, float angle)
	{
		m_pManager->SetRotation(handle, ::EsVec3(vAxis.x, vAxis.y, vAxis.z), angle);
	}

	//回転を指定する(クォータニオンを使用)
	void SetRotation(::EsHandle handle, D3DXQUATERNION qua)
	{
		D3DXVECTOR3 axis;
		float angle;

		//クォータニオンから軸と角度に変換
		D3DXQuaternionToAxisAngle(&qua, &axis, &angle);

		m_pManager->SetRotation(handle, ::EsVec3(axis.x, axis.y, axis.z), angle);
	}

	//サイズを指定する
	void SetScale(::EsHandle handle, D3DXVECTOR3 scale)
	{
		m_pManager->SetScale(handle, scale.x, scale.y, scale.z);
	}

private:

	//ビュー行列を設定
	void SetViewMatrix(const D3DXMATRIX& mView);
	//プロジェクション行列を設定
	void SetProjectionMatrix(const D3DXMATRIX& mProj);

private:
	//エフェクトを動作させるために必要
	::EsManagerRef		m_pManager;
	::EsRendererRef		m_pRenderer;

	//エフェクトの種類ごとに必要
	std::unordered_map< std::string, EsEffectRef> m_pEffect;

};

