#include "CollisionCapsule.h"

CollisionCapsule::CollisionCapsule(
    ICollisionListener* pListener,
    const D3DXVECTOR3 syncPosition,
    ColliderTag tag,
    float radius,
    const D3DXVECTOR3 localOffsetA,
    const D3DXVECTOR3 localOffsetB,
    const D3DXVECTOR3 localOffset
)
    : CollisionBase(pListener, syncPosition, tag, localOffset)
    , m_Radius(radius)
    , m_WorldCapsule({})
{
    m_LocalOffsetA = localOffsetA;
    m_LocalOffsetB = localOffsetB;
}


void CollisionCapsule::UpdateWorldPosition()
{
    // 親オブジェクトのワールド行列を取得
    CGameObject* obj = dynamic_cast<CGameObject*>(GetListener());

    if (!obj)
    {
        // 親がいなければ判定不能
        SetActive(false);
        m_WorldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        return;
    }

    const D3DXMATRIX mWorld = obj->GetWorldMatrix();

    D3DXVECTOR3 totalOffSetA =  m_LocalOffsetA + m_MoveOffsetA;
    D3DXVECTOR3 totalOffSetB =  m_LocalOffsetB + m_MoveOffsetB;

    // 軸線分の始点Aと終点Bをワールド行列で変換
    // D3DXVec3TransformCoord は D3DXMATRIXの回転、スケール、平行移動を全て適用
    D3DXVec3TransformCoord(&m_WorldCapsule.StartPoint, &totalOffSetA, &mWorld);
    D3DXVec3TransformCoord(&m_WorldCapsule.EndPoint, &totalOffSetA, &mWorld);

    // 半径の設定
    m_WorldCapsule.Rad = m_Radius;

    // CollisionBase の m_WorldPosition をカプセルの中心で更新
    m_WorldPosition = (m_WorldCapsule.StartPoint + m_WorldCapsule.EndPoint) / 2.0f;

    // 軸線分ベクトルと長さの計算
    D3DXVECTOR3 vAxisWorld = m_WorldCapsule.EndPoint - m_WorldCapsule.StartPoint;
    m_DrawLength = D3DXVec3Length(&vAxisWorld);

    // 描画中心位置の設定
    m_DrawCenterPosition = m_WorldPosition;

    // 回転の計算
    D3DXVECTOR3 vLocalAxis(0.0f, 1.0f, 0.0f);
    D3DXQUATERNION qRotation;

    // 軸が潰れている場合は、回転計算で不正な値が出ないように早期リターン
    if (m_DrawLength < 1e-6f) {
        D3DXQuaternionIdentity(&m_DrawRotation);
        return;
    }

    // 軸ベクトルを正規化
    D3DXVECTOR3 vNormalizedAxis;
    D3DXVec3Normalize(&vNormalizedAxis, &vAxisWorld);

    // 標準の最短クォータニオン回転ロジック
    D3DXVECTOR3 vAxis;
    float fDot = D3DXVec3Dot(&vLocalAxis, &vNormalizedAxis);

    if (fDot > 0.999f) D3DXQuaternionIdentity(&qRotation);
    else if (fDot < -0.999f)
    {
        D3DXVECTOR3 vAxis180(0.0f, 0.0f, 1.0f);
        D3DXQuaternionRotationAxis(&qRotation, &vAxis180, D3DX_PI);
    }
    else
    {
        D3DXVec3Cross(&vAxis, &vLocalAxis, &vNormalizedAxis);
        D3DXVec3Normalize(&vAxis, &vAxis);
        float fAngle = acosf(fDot);
        D3DXQuaternionRotationAxis(&qRotation, &vAxis, fAngle);
    }

    m_DrawRotation = qRotation;
}

