#include "stdafx.h"
#include "CPig.h"
#include "CFarmland.h"



CPig::CPig(CGameWorld & _rGameWorld, CFarmland* _pFarmland, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f, PIG_WIDTH, PIG_HEIGHT),
	m_pFarmland(_pFarmland)
{
	SetRenderLayer(10.f);
}

CPig::~CPig()
{
}

void CPig::Ready(void)
{
}

int CPig::Update(float _fDeltaTime)
{
	return 0;
}

void CPig::LateUpdate(void)
{
}

void CPig::Render(CCamera * _pCamera)
{
}

bool CPig::Cropping(float _fDeltaTime)
{
	return true;
}
