#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"
#include "CSceneMgr.h"
#include "CSpace.h"
// #include "CBitmapMgr.h"


CGameWorld::CGameWorld()
	:
	m_pGraphicDevice(CGraphicDevice::GetInstance()),
	m_pTimer(new CTimer(CTimer::TIMER_TYPE_WINDOWS)),
	m_pSceneManager(new CSceneMgr(*this)),
	// ViewSpace는 렌더 컬링할때 쓰일 수 있다.
	m_pViewSpace(new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX), static_cast<int>(WINCY)))
{
	srand((unsigned int)time(nullptr));

	if (m_pGraphicDevice) m_pGraphicDevice->GenerateGraphicDevice();
	if (m_pTimer) m_pTimer->Reset();
}


CGameWorld::~CGameWorld()
{
	SafelyDeleteObj(m_pTimer);
	SafelyDeleteObj(m_pSceneManager);
	SafelyDeleteObj(m_pViewSpace);
	m_pGraphicDevice->DestroyInstance();
}

bool CGameWorld::ConfirmValidScene(void)
{
	return m_pSceneManager->ConfirmValidScene();
}

void CGameWorld::RunTick(void)
{
	if (m_pTimer) m_pTimer->RunTick();
}

void CGameWorld::StartRender(void)
{
	m_pGraphicDevice->BeginRender();
}

void CGameWorld::EndRender(void)
{
	m_pGraphicDevice->EndRender();
}