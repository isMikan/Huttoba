#include "CGround.h"

CGround::CGround()
{
}

CGround::~CGround()
{
}

void CGround::Update()
{
	//地面をスクロールさせる
	m_vPosition.z -= 0.2f;
	if (m_vPosition.z < -100.f) {
		m_vPosition.z = 0.f;
	}
}
