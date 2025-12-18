#pragma once

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>
#undef min			//std::minと競合するのでwindows.hのminを無効化.
#undef max			//std::maxと競合するのでwindows.hのmaxを無効化.

#include <algorithm>	//std::max,std::minを使用する.

#include <crtdbg.h>

//DirectX9
#include <d3dx9.h>
//DirectX11
#include <D3DX11.h>
#include <D3D11.h>
//DirectX10
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include <DirectXMath.h>

#include "MyMacro.h"

//ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" )
//DirectX9
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )
//DirectX11
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
//DirectX10
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.

//=================================================
//	定数.
//=================================================
//#define WND_W 1280	//←定数宣言で#defineは使わない.
const int WND_W = 1280;	//ウィンドウの幅.
const int WND_H = 720;	//ウィンドウの高さ.
const int FPS = 60;		//フレームレート.

constexpr float TIME_LIMIT = 60.f;	//ゲームの制限時間

//最大数
static constexpr int Enemy_Max = 3;
static constexpr int Explosion_Max = 20;
static constexpr int Shadow_Max = 50;	//影.
static constexpr int Ground_Max = 4;	//地面.
static constexpr int Player_Max = 4;	//プレイヤー.
static constexpr int Gauge_Max = 40;	//ゲージとフレーム.

//弾のインターバル
static constexpr float Player_Shot_Interval = 15;

//=================================================
//	構造体
//=================================================
//カメラ情報構造体.
struct CAMERA
{
	D3DXVECTOR3	vPosition;	//視点.
	D3DXVECTOR3 vLook;		//注視点.
};
//ライト情報.
struct LIGHT
{
	D3DXVECTOR3	Position;	//位置.
	D3DXVECTOR3	vDirection;	//方向.
	D3DXMATRIX	mRotation;	//回転行列.
	float		fIntensity;	//強度(明るさ).
};

//ゲージの構造体.
struct Gauge
{
	float remaining;	//残り.
	float max;			//最大.
};

//オブジェクトの色の構造体.
struct ObjectColor
{
	D3DXVECTOR4	diffuse;	//拡散反射.
	D3DXVECTOR4	ambient;	//環境光.
	D3DXVECTOR4	specular;	//鏡面反射.
};

//-----------------------
//  列挙
//-----------------------
//シーン列挙
enum EnSceneList
{
	Title = 0,
	Standby,
	GameMain,
	Result,

	Max,
	None = -1
};

enum class SkinMeshList
{
	Zako = 0,
	Chick
};

enum class StaticMeshList
{
	FirstFallGround = 0,
	SecondFallGround,
	ThirdFallGround,
	SafeGround,
	Player,
	PBody,
	PHead,
	PHand,
	RoboA,
	RoboB,
	Bullet,
	BSphere,
	BCapsule,
	BWidthCapsule,
	Haetataki,
	Bomb,
	Fun,
	Mushroom,
	TrackingRobot,
	Boomerang,
	Magnet,
	SmashBat,

	ExplosionCol,
	FunCol,
	TestPlayer,
	BoomerangCol,
	PickUpCol,
	HaetatakiCol,
	SmashBatCol,

	floor1,
	floor2,
	floor3,
	floor4,
};

enum class Sprite3DList
{
	Explosion,
	Shadow,
};

enum class Sprite2DList
{
	Fade = 0,
	Sky,
	GaugeFrame,
	Gauge,
	Font_Title,
	Font_Result,
	Font_ReadyGo,
	Font_Finish,
	Font_UI_Common,
	Font_UI_Ready,
	UI_Selector,
	Clock,	
	TimeNumber,	
};

//サウンドリスト列挙型
enum class enSoundList
{
	BGM_Bonus,
	BGM_SceneMain,
	BGM_SceneTitle,
	BGM_SceneStanby,
	BGM_SceneResult,

	SE_Jump,
	SE_Clear,
	SE_Pickup,
	SE_AttackHand,
	SE_Whiff,
	SE_Down,
	SE_Knockback, 
	SE_Knockdown, 
	SE_HitHand,
	SE_Warning,
	SE_HitHaetataki,
	SE_MissHaetataki,
	SE_BoomerangCharge,
	SE_BoomerangMaxCharge,
	SE_BoomerangThrow,
	SE_SmashBatHit,
	SE_FunUse,
	SE_DestroyItem,
	SE_EndSceneMain,

	//音が増えたら「ここ」に追加
	max,
};

enum class ItemID
{
	Haetataki = 0,
	Bomb,
	Fun,
	Mushroom,
	SmashBat,
	TrackingRobot,
	Boomerang,
	Magnet,

	Max,
	None = -1,
};

enum class GroundTag
{
	SafeGround = 0,		//落ちない中央の地面
	ThirdFallGround,	//三番目に落ちる地面
	SecondFallGround,	//二番目に落ちる地面
	FirstFallGround,	//一番目に落ちる地面

	None = -1
};


