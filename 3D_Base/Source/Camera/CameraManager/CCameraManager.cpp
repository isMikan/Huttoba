#include "CCameraManager.h"

CCameraManager::CCameraManager()
	: m_pCamera		( std::make_unique<CCamera>() )

	, m_Camera		()
	, m_Light		()
{
}

CCameraManager::~CCameraManager()
{
}

//======================================================================
// 	   “à•”‚ÅŒÄ‚Ño‚·ŠÖ”.
//======================================================================

//--- XVˆ— ---.
void CCameraManager::Update_Internal()
{
	m_pCamera->UpdateView(m_Camera, m_mView);
	m_pCamera->UpdateProjection(m_mProj);
}