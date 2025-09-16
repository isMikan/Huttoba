#include "CPlayer.h"
#include "Sound/CSoundManager.h"

#include "PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerState/ActionState/PlayerActionIdle/CPlayerActionIdle.h"

#include "PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"

#include "PlayerState/ActionState/PlayerHandAttack/CPlayerHandAttack.h"
#include "PlayerState/ActionState/PlayerPickupItem/CPlayerPickupItem.h"
#include "PlayerState/ActionState/PlayerThrowItem/CPlayerThrowItem.h"
#include "PlayerState/ActionState/PlayerPushed/CPlayerPushed.h"

CPlayer::CPlayer(int index)
	: m_pInput			( std::make_unique<CInput>(index) )

	, m_pHead			( std::make_unique<CPlayerHead>() )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdleState>( 0.f, 0.f ) )
	, m_pTurnState		( std::make_unique<CPlayerTurnIdleState>( 0.f, 0.f ) )
	, m_pActionState	( std::make_unique<CPlayerActionIdle>() )

	, m_IsMoving		( false )
	, m_IsRotating		( false )
	, m_IsHoldingItem	( false )
	, m_IsAttacking		( false )

	, m_HitInfo			()
{
	SetCharacterDefault(index);

	SetPlayerInputBinding();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	GetPlayerHead().SetPosition(SetHeadPos());

	m_pInput->Update();

	HandleInput();

	m_pMoveState->Update(*this);

	m_pTurnState->Update(*this);

	m_pActionState->Update(*this);

	CCharacter::Update();
}

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CCharacter::Draw( View, Proj, Light, Camera );
}

void CPlayer::HandleInput()
{
	float x = 0.f;	//x軸.
	float z = 0.f;	//z軸.

	if (m_pInput->IsRepeat(Action::MoveUp))		z += 1.f;
	if (m_pInput->IsRepeat(Action::MoveDown))	z -= 1.f;
	if (m_pInput->IsRepeat(Action::MoveLeft))	x -= 1.f;
	if (m_pInput->IsRepeat(Action::MoveRight))	x += 1.f;

	//接続されていたら数値を受け取る.
	if (m_pInput->IsConnect())
	{
		x = m_pInput->GetLeftSthikX();
		z = m_pInput->GetLeftSthikY();
	}

	SetMoveState(std::make_unique<CPlayerMoveState>(x, z));
	SetTurnState(std::make_unique<CPlayerTurnState>(x, z));

	//アイテムを持っていないなら攻撃.
	if (m_pInput->IsDown(Action::Attack) && !m_IsHoldingItem
		&& !m_IsAttacking)
	{
		SetActionState(std::make_unique<CPlayerHandAttack>());
	}
	//アイテムを持っていないなら拾う.
	if (m_pInput->IsDown(Action::ToggleItem) && !m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerPickupItem>());
	}
	//アイテムを持っているなら捨てる.
	else if (m_pInput->IsDown(Action::ToggleItem) && m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerThrowItem>());
	}

	if (m_HitInfo.isHit == true
		&& m_HitInfo.force > 0.f)
	{
		SetActionState(std::make_unique<CPlayerPushed>());
	}
}

//移動状態を設定する関数.
void CPlayer::SetMoveState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pMoveState, std::move(newState));
}

//回転状態を設定する関数.
void CPlayer::SetTurnState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pTurnState, std::move(newState));
}

//行動状態を設定する関数.
void CPlayer::SetActionState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pActionState, std::move(newState));
}

//状態遷移の処理関数.
void CPlayer::ChangeState(
	std::unique_ptr<CPlayerState>& currentState,
	std::unique_ptr<CPlayerState> newState)
{
	if (currentState != nullptr)
	{
		//状態の終了処理.
		currentState->Exit(*this);
	}

	//新しい状態にする.
	currentState = std::move(newState);

	if (currentState != nullptr)
	{
		//状態の開始処理.
		currentState->Enter(*this);
	}
}

D3DXVECTOR3 CPlayer::SetHeadPos()
{
	LocalAxes axes = GetLocalAxes();

	D3DXVECTOR3 headOffsetPos = GetPlayerHead().GetOffsetPos();

	//方向に合わせて位置を調整.
	headOffsetPos =
		axes.right * headOffsetPos.x +
		axes.up * headOffsetPos.y +
		axes.forward * headOffsetPos.z;

	D3DXVECTOR3 headPos = m_vPosition;

	//頭の位置.
	headPos = m_vPosition + headOffsetPos;

	return headPos;
}

//プレイヤーの正面方向を取得するための関数.
D3DXVECTOR3 CPlayer::GetForward()
{
	D3DXMATRIX rot;
	//クォータニオンをマトリクスに変換.
	D3DXMatrixRotationQuaternion(&rot, &m_vQuaternion);

	D3DXVECTOR3 forward(0, 0, 1); //z軸の正面方向を基準にする. 
	D3DXVec3TransformCoord(&forward, &forward, &rot);
	return forward;
}

//ローカル座標軸を取得.
CPlayer::LocalAxes CPlayer::GetLocalAxes()
{
	//ローカル軸.
	LocalAxes axes;

	//ローカル軸をクォータニオンにして計算.
	axes.right =	RotateVectorByQuat(D3DXVECTOR3(1, 0, 0), m_vQuaternion);
	axes.up =		RotateVectorByQuat(D3DXVECTOR3(0, 1, 0), m_vQuaternion);
	axes.forward =	RotateVectorByQuat(D3DXVECTOR3(0, 0, 1), m_vQuaternion);
	//正規化.
	D3DXVec3Normalize(&axes.right, &axes.right);
	D3DXVec3Normalize(&axes.up, &axes.up);
	D3DXVec3Normalize(&axes.forward, &axes.forward);

	return axes;
}

//クォータニオンによるベクトル回転の関数.
D3DXVECTOR3 CPlayer::RotateVectorByQuat(
	D3DXVECTOR3 vector, D3DXQUATERNION quat)
{
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//ベクトルをクォータニオンに変換.
	D3DXQUATERNION vecQuat(vector.x, vector.y, vector.z, 0.f);

	//逆元.
	D3DXQUATERNION inverseRot;
	//逆元を求める(共役(-x,-y-,z,w)をベクトルサイズの二乗で割る).
	D3DXQuaternionInverse(&inverseRot, &quat);

	//途中結果(v * -q).
	D3DXQUATERNION temp;
	//ベクトルと逆元をかける(回転が歪まないように).
	D3DXQuaternionMultiply(&temp, &inverseRot, &vecQuat);
	//最終結果(q * {v * -q}).
	D3DXQUATERNION result;
	D3DXQuaternionMultiply(&result, &temp, &quat);

	//ベクトル分だけ返す.
	return D3DXVECTOR3(result.x, result.y, result.z);
}

//プレイヤーの初期角度から傾きを計算する関数..
D3DXQUATERNION CPlayer::TiltedQuat(
	D3DXQUATERNION baseQuat, D3DXVECTOR3 localAxes, float tiltAngle)
{
	//傾き用クォータニオン.
	D3DXQUATERNION tilt;
	//マトリクスをクォータニオンに変換.
	D3DXQuaternionRotationAxis(&tilt, &localAxes, tiltAngle);
	D3DXQUATERNION quat;
	D3DXQuaternionMultiply(&quat, &baseQuat, &tilt); //基準の姿勢に傾きを掛ける.
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	return quat;
}

//押された時の移動量を計算する関数.
D3DXVECTOR3 CPlayer::Knockback()
{
	D3DXVECTOR3 dir = m_vPosition - m_HitInfo.position;

	D3DXVec3Normalize(&dir, &dir);

	D3DXVECTOR3 pos = dir * (m_HitInfo.force);

	return pos;
}

void CPlayer::SetCharacterDefault(int index)
{
	std::array<D3DXVECTOR3, Player_Max> playerPos;							//プレイヤーの位置.
	std::array<CStaticMeshObject::ObjectColor, Player_Max>	playerColor;	//プレイヤーの色.

	switch (index)
	{
	case 0:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 5.f);
		playerColor[index] = {
			D3DXVECTOR4(1.f, 0.f, 0.f, 1.f), // 赤
			D3DXVECTOR4(0.5f, 0.f, 0.f, 1.f), // 少し暗めの赤
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),
		};

		break;
	case 1:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 5.f);
		playerColor[index] = {
			D3DXVECTOR4(0.f, 0.f, 1.f, 1.f),  // 青
			D3DXVECTOR4(0.f, 0.f, 0.5f, 1.f),  // 少し暗めの青
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 2:
		playerPos[index] = D3DXVECTOR3(-5.f, 0.f, 10.f);
		playerColor[index] = {
			D3DXVECTOR4(1.0f, 0.5f, 0.f, 1.f),  // オレンジ
			D3DXVECTOR4(0.5f, 0.3f, 0.f, 1.f), // 少し暗めのオレンジ
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	case 3:
		playerPos[index] = D3DXVECTOR3(5.f, 0.f, 10.f);
		playerColor[index] = {
			D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.f),  // 緑
			D3DXVECTOR4(0.0f, 0.5f, 0.0f, 1.f),  // 少し暗めの緑
			D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f),  // 白っぽい光沢
		};

		break;
	default:
		break;
	}

	m_vPosition = playerPos[index];
	m_ObjColor = playerColor[index];
}

//キーバインドを設定する関数.
void CPlayer::SetPlayerInputBinding()
{
	//キーボード操作.
	m_pInput->BindKey(Action::MoveUp,		InputBinding(InputDevice::Keyboard, VK_UP));		//上移動.
	m_pInput->BindKey(Action::MoveDown,		InputBinding(InputDevice::Keyboard, VK_DOWN));		//下移動.
	m_pInput->BindKey(Action::MoveLeft,		InputBinding(InputDevice::Keyboard, VK_LEFT));		//左移動.
	m_pInput->BindKey(Action::MoveRight,	InputBinding(InputDevice::Keyboard, VK_RIGHT));		//右移動.
	m_pInput->BindKey(Action::Attack,		InputBinding(InputDevice::Keyboard, 'Z'));			//攻撃.
	m_pInput->BindKey(Action::ToggleItem,	InputBinding(InputDevice::Keyboard, 'X'));			//拾う/捨てる.

	//コントローラ操作.
	m_pInput->BindKey(Action::Attack,		InputBinding(InputDevice::GamePad, CXInput::B));	//攻撃.
	m_pInput->BindKey(Action::ToggleItem,	InputBinding(InputDevice::GamePad, CXInput::A));	//拾う/捨てる.
}
