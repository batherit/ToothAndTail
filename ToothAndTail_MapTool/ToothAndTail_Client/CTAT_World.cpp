#include "stdafx.h"
#include "CTAT_World.h"
#include "CSceneMgr.h"
#include "CTestScene.h"
#include "CTimer.h"
#include "CCamera.h"


CTAT_World::CTAT_World()
{
}


CTAT_World::~CTAT_World()
{
	Release();
}

LRESULT CTAT_World::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void CTAT_World::Ready(void)
{
	GetSceneManager()->SetNextScene(new CTestScene(*this));		// 초기씬 세팅
	SetMainCamera(new CCamera(*this, nullptr, 0.f, 0.f));		// 메인 카메라 세팅
}

void CTAT_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();

	GetSceneManager()->Update(fDeltaTime);
	GetMainCamera()->Update(fDeltaTime);
}

void CTAT_World::LateUpdate(void)
{
	GetSceneManager()->LateUpdate();
	GetMainCamera()->LateUpdate();
}

void CTAT_World::Render(void)
{
	StartRender();
	GetSceneManager()->Render(GetMainCamera());
	EndRender();
}

void CTAT_World::Release(void)
{
}
