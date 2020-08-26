#include "stdafx.h"
#include "CTestScene.h"
#include "CGameWorld.h"
#include "CCom_Commoner.h"


CTestScene::CTestScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld)
{
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::ResetScene(void)
{
	Release();
	m_rGameWorld.GetListObjs().emplace_back(new CCom_Commoner(m_rGameWorld, -200.f, 0.f, L"COM_MILITARY", D3DCOLOR_ARGB(255, 0, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCom_Commoner(m_rGameWorld, -100.f, 0.f, L"COM_CAPITALIST", D3DCOLOR_ARGB(255, 0, 0, 255)));
	m_rGameWorld.GetListObjs().emplace_back(new CCom_Commoner(m_rGameWorld, 0.f, 0.f, L"COM_CLERGY", D3DCOLOR_ARGB(255, 255, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCom_Commoner(m_rGameWorld, 100.f, 0.f, L"COM_COMMONER", D3DCOLOR_ARGB(255, 255, 0, 0)));
}

int CTestScene::Update(float _fDeltaTime)
{
	for (auto& rObj : m_rGameWorld.GetListObjs()) {
		rObj->Update(_fDeltaTime);
	}
	return 0;
}

void CTestScene::LateUpdate(void)
{
	for (auto& rObj : m_rGameWorld.GetListObjs()) {
		rObj->LateUpdate();
	}
}

void CTestScene::Release(void)
{
	SafelyDeleteObjs(m_rGameWorld.GetListObjs());
	m_pSpriteObj = nullptr;
}

void CTestScene::Render(CCamera * _pCamera)
{
	for (auto* rObj : m_rGameWorld.GetListObjs()) {
		rObj->Render(_pCamera);
	}
}
