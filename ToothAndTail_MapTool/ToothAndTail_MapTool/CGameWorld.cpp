#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"
#include "CSpace.h"
// #include "CBitmapMgr.h"


CGameWorld::CGameWorld()
	:
	m_pGraphicDevice(CGraphicDevice::GetInstance()),
	m_pTimer_Main(new CTimer(CTimer::TIMER_TYPE_WINDOWS)),
	// ViewSpace는 렌더 컬링할때 쓰일 수 있다.
	m_pViewSpace_Main(new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX), static_cast<int>(WINCY)))
{
	srand((unsigned int)time(nullptr));

	if (m_pGraphicDevice) m_pGraphicDevice->GenerateGraphicDevice();
	if (m_pTimer_Main) m_pTimer_Main->Reset();
	m_vecRenderObjs_Main.reserve(1000);
}


CGameWorld::~CGameWorld()
{
	SafelyDeleteObj(m_pTimer_Main);
	SafelyDeleteObj(m_pViewSpace_Main);
	m_vecRenderObjs_Main.clear();
	m_vecRenderObjs_Main.shrink_to_fit();
	m_pGraphicDevice->DestroyInstance();
	m_pCamera_Main = nullptr;
}

void CGameWorld::RunTick(void)
{
	if (m_pTimer_Main) m_pTimer_Main->RunTick();
}

void CGameWorld::StartRender(void)
{
	m_pGraphicDevice->BeginRender();
}

void CGameWorld::EndRender(void)
{
	m_pGraphicDevice->EndRender();
}

void CGameWorld::RenderListObjs(CCamera* _pCamera, bool (*funcComp)(CObj* obj1, CObj* obj2))
{
	m_vecRenderObjs_Main.clear();
	for (auto& pObj : GetListObjs()) {
		pObj->RegisterToRenderList(m_vecRenderObjs_Main);
	}
	sort(m_vecRenderObjs_Main.begin(), m_vecRenderObjs_Main.end(), funcComp);
	
	for (auto& pObj : m_vecRenderObjs_Main) {
		pObj->Render(_pCamera);
	}
}

