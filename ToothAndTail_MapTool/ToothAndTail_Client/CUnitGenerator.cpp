#include "stdafx.h"
#include "CUnitGenerator.h"



CUnitGenerator::CUnitGenerator(CGameWorld& _rGameWorld)
	:
	CObj(_rGameWorld, 0.f, 0.f)
{
}

CUnitGenerator::~CUnitGenerator()
{
}

void CUnitGenerator::Ready(void)
{
}

int CUnitGenerator::Update(float _fDeltaTime)
{
	// TODO : 
	return 0;
}

void CUnitGenerator::LateUpdate(void)
{
}

void CUnitGenerator::Release(void)
{
}
