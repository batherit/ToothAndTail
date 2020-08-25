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
	m_pSpriteObj = new CCom_Commoner(m_rGameWorld, 0.f, 0.f);
	m_rGameWorld.GetListObjs().emplace_back(m_pSpriteObj);
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
