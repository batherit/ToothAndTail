#include "stdafx.h"
#include "CTestScene.h"



CTestScene::CTestScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld)
{
}

CTestScene::~CTestScene()
{
}

void CTestScene::ResetScene(void)
{
}

int CTestScene::Update(float _fDeltaTime)
{
	return 0;
}

void CTestScene::LateUpdate(void)
{
}

void CTestScene::Release(void)
{
}
