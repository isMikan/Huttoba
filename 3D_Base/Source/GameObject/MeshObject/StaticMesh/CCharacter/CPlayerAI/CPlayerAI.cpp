#include "CPlayerAI.h"
#include "Sound/CSoundManager.h"

#include "PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "PlayerState/PlayerMoveState/PlayerMoveState/CPlayerMoveState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnState/CPlayerTurnState.h"

#include "PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"
#include "PlayerState/PlayerActionState/PlayerPickupState/CPlayerPickupState.h"
#include "PlayerState/PlayerActionState/PlayerThrowState/CPlayerThrowState.h"
#include "PlayerState/PlayerActionState/PlayerPushedState/CPlayerPushedState.h"

CPlayerAI::CPlayerAI(int index)
	: m_pInput			( std::make_unique<CInput>(index) )

	, m_pHead			( std::make_unique<CPlayerHead>() )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdleState>( 0.f, 0.f ) )
	, m_pTurnState		( std::make_unique<CPlayerTurnIdleState>( 0.f, 0.f ) )
	, m_pActionState	( std::make_unique<CPlayerActionIdleState>() )

	, m_IsMoving		( false )
	, m_IsRotating		( false )
	, m_IsHoldingItem	( false )
	, m_IsAttacking		( false )

	, m_HitInfo			()
{
	SetCharacterDefault(index);

	SetPlayerInputBinding(index);
}

CPlayerAI::~CPlayerAI()
{
}

//----- 毎フレームの動作する関数 -----.
void CPlayerAI::Update()
{
	GetPlayerHead().SetPosition(SetHeadPos());

	m_pInput->Update();

	HandleInput();

	m_pMoveState->Update(*this);

	m_pTurnState->Update(*this);

	m_pActionState->Update(*this);

	CCharacter::Update();
}

//----- 毎フレームの描画する関数 -----.
void CPlayerAI::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CCharacter::Draw( View, Proj, Light, Camera );
}

//----- キー操作 -----.
void CPlayerAI::HandleInput()
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

	if(!m_IsStopping)
	{
		SetMoveState(std::make_unique<CPlayerMoveState>(x, z));
		SetTurnState(std::make_unique<CPlayerTurnState>(x, z));
	}

	//アイテムを持っていないなら攻撃.
	if (m_pInput->IsDown(Action::Attack) && !m_IsHoldingItem
		&& !m_IsAttacking)
	{
		SetActionState(std::make_unique<CPlayerHandAttackState>());
	}
	//アイテムを持っていないなら拾う.
	if (m_pInput->IsDown(Action::ToggleItem) && !m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerPickupState>());
	}
	//アイテムを持っているなら捨てる.
	else if (m_pInput->IsDown(Action::ToggleItem) && m_IsHoldingItem)
	{
		SetActionState(std::make_unique<CPlayerThrowState>());
	}
	//押された時の処理.
	if (m_HitInfo.isHit == true
		&& m_HitInfo.force > 0.f)
	{
		SetActionState(std::make_unique<CPlayerPushedState>());
	}
}

//----- 移動状態を設定する関数 -----.
void CPlayerAI::SetMoveState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pMoveState, std::move(newState));
}

//----- 回転状態を設定する関数 -----.
void CPlayerAI::SetTurnState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pTurnState, std::move(newState));
}

//----- 行動状態を設定する関数 -----.
void CPlayerAI::SetActionState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pActionState, std::move(newState));
}

//----- 状態遷移の処理関数 -----.
void CPlayerAI::ChangeState(
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

//----- 頭の位置を設定するために計算する関数 -----.
D3DXVECTOR3 CPlayerAI::SetHeadPos()
{
	//ローカル軸を取得.
	LocalAxes axes = GetLocalAxes();

	//調整位置を取得.
	D3DXVECTOR3 headOffsetPos = GetPlayerHead().GetOffsetPos();

	//方向に合わせて位置を調整.
	headOffsetPos =
		axes.right * headOffsetPos.x +
		axes.up * headOffsetPos.y +
		axes.forward * headOffsetPos.z;

	//頭の位置をプレイヤーの位置に合わせる.
	D3DXVECTOR3 headPos = m_vPosition;

	//上に調整.
	headPos = m_vPosition + headOffsetPos;

	return headPos;
}

//----- プレイヤーの正面方向を取得するための関数 -----.
D3DXVECTOR3 CPlayerAI::GetForward()
{
	D3DXMATRIX rot;
	//クォータニオンをマトリクスに変換.
	D3DXMatrixRotationQuaternion(&rot, &m_vQuaternion);

	D3DXVECTOR3 forward(0, 0, 1); //z軸の正面方向を基準にする. 
	D3DXVec3TransformCoord(&forward, &forward, &rot);
	return forward;
}

//----- ローカル座標軸を取得 -----.
CPlayerAI::LocalAxes CPlayerAI::GetLocalAxes()
{
	//ローカル軸.
	LocalAxes axes{};

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

//----- クォータニオンによるベクトル回転の関数 -----.
D3DXVECTOR3 CPlayerAI::RotateVectorByQuat(
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

//----- プレイヤーの初期角度から傾きを計算する関数 -----.
D3DXQUATERNION CPlayerAI::TiltedQuat(
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

//----- 押された時の移動量を計算する関数 -----.
D3DXVECTOR3 CPlayerAI::Knockback() const
{
	//押されるベクトル.
	D3DXVECTOR3 dir = m_vPosition - m_HitInfo.position;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);
	//方向へ吹き飛び量分の位置へ.
	D3DXVECTOR3 pos = dir * m_HitInfo.force;

	return pos;
}

//----- 攻撃を受けた時のの移動量 -----.
D3DXVECTOR3 CPlayerAI::GetVelocity() const
{
	//飛ぶベクトル.
	D3DXVECTOR3 dir = m_vPosition - m_HitInfo.position;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);

	//角度60度上方向.
	float angle = D3DXToRadian(60.f);
	float speed = m_HitInfo.force;	//吹き飛ばし量を速度とする.

	D3DXVECTOR3 velocity{};
	velocity.x = cos(angle) * speed * dir.x;	//x軸方向に.
	velocity.z = cos(angle) * speed * dir.z;	//z軸方向に.
	velocity.y = sin(angle) * speed;			

	return velocity;
}

//----- 数値の領域を指定する関数 -----.
float CPlayerAI::Clamp(float value, float min, float max)
{
	//最小値より小さかったら最小値を返す.
	if (value < min) return min;
	//最大値より大きかったら最大値を返す.
	if (value > max) return max;

	return value;
}

//----- 角度を0～360度にする関数 -----.
float CPlayerAI::WrapAngle(float value)
{
	//360度以上なら引く.
	if (value >= 2.f * D3DX_PI) return value - 2.f * D3DX_PI;
	//0度以上なら足す.
	if (value < 0.f) return value + 2.f * D3DX_PI;

	return value;
}