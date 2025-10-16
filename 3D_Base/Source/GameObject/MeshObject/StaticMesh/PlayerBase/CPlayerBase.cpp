#include "CPlayerBase.h"

#include "PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerBase::CPlayerBase( int index )
	: m_pHead			( std::make_unique<CPlayerHead>() )
	, m_pRightHand		( std::make_unique<CPlayerRightHand>() )
	, m_pLeftHand		( std::make_unique<CPlayerLeftHand>() )

	, m_pMoveState		( std::make_unique<CPlayerMoveIdleState>( 0.f, 0.f ) )
	, m_pTurnState		( std::make_unique<CPlayerTurnIdleState>( 0.f, 0.f ) )
	, m_pActionState	( std::make_unique<CPlayerActionIdleState>() )

	, m_IsMoving		( false )
	, m_IsRotating		( false )
	, m_IsHoldingItem	( false )
	, m_IsAttacking		( false )
	, m_IsStopping		( false )

	, m_HitInfo			()
{
}

CPlayerBase::~CPlayerBase()
{
}

void CPlayerBase::Update()
{
	CStaticMeshObject::Update();
}

void CPlayerBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}

//--- 移動状態を設定する関数 ---.
void CPlayerBase::SetMoveState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pMoveState, std::move(newState));
}

//--- 回転状態を設定する関数 ---.
void CPlayerBase::SetTurnState(std::unique_ptr< CPlayerState> newState)
{
	ChangeState(m_pTurnState, std::move(newState));
}

//--- 行動状態を設定する関数 ---.
void CPlayerBase::SetActionState(std::unique_ptr<CPlayerState> newState)
{
	ChangeState(m_pActionState, std::move(newState));
}

//--- 状態遷移の処理関数 ---.
void CPlayerBase::ChangeState(
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

//--- 位置を設定するために計算する関数 ---.
D3DXVECTOR3 CPlayerBase::GetObjectPos(D3DXVECTOR3 offset)
{
	//ローカル軸を取得.
	LocalAxes axes = GetLocalAxes();

	//方向による位置を調整.
	offset =
		axes.right * offset.x +
		axes.up * offset.y +
		axes.forward * offset.z;

	//プレイヤーに合わせて調整.
	D3DXVECTOR3 objPos = m_vPosition + offset;

	return objPos;
}

//--- ローカル座標軸を取得 ---.
CPlayerBase::LocalAxes CPlayerBase::GetLocalAxes()
{
	//ローカル軸.
	LocalAxes axes;

	//ローカル軸をクォータニオンにして計算.
	axes.right = RotateVectorByQuat(D3DXVECTOR3(1, 0, 0), m_vQuaternion);
	axes.up = RotateVectorByQuat(D3DXVECTOR3(0, 1, 0), m_vQuaternion);
	axes.forward = RotateVectorByQuat(D3DXVECTOR3(0, 0, 1), m_vQuaternion);
	//正規化.
	D3DXVec3Normalize(&axes.right, &axes.right);
	D3DXVec3Normalize(&axes.up, &axes.up);
	D3DXVec3Normalize(&axes.forward, &axes.forward);

	return axes;
}

//--- クォータニオンによるベクトル回転の関数 ---.
D3DXVECTOR3 CPlayerBase::RotateVectorByQuat(
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

//--- プレイヤーの初期角度から傾きを計算する関数 ---.
D3DXQUATERNION CPlayerBase::TiltedQuat(
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

//--- 押された時の移動量を計算する関数 ---.
D3DXVECTOR3 CPlayerBase::Pushed()
{
	//押されるベクトル.
	D3DXVECTOR3 dir = m_vPosition - m_HitInfo.position;
	//正規化.
	D3DXVec3Normalize(&dir, &dir);
	//方向へ吹き飛び量分の位置へ.
	D3DXVECTOR3 pos = dir * m_PushForce;

	return pos;
}

//--- 攻撃を受けた時のの移動量 ---.
D3DXVECTOR3 CPlayerBase::GetVelocity()
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

//--- 数値の領域を指定する関数 ---.
float CPlayerBase::Clamp(float value, float min, float max)
{
	//最小値より小さかったら最小値を返す.
	if (value < min) return min;
	//最大値より大きかったら最大値を返す.
	if (value > max) return max;

	return value;
}

//--- 角度を0～360度にする関数 ---.
float CPlayerBase::WrapAngle(float value)
{
	//360度以上なら引く.
	if (value >= 2.f * D3DX_PI) return value - 2.f * D3DX_PI;
	//0度以上なら足す.
	if (value < 0.f) return value + 2.f * D3DX_PI;

	return value;
}
