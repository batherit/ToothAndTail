#include "stdafx.h"
#include "CFarmland.h"



CFarmland::CFarmland(CGameWorld & _rGameWorld, float _fX, float _fY, CFarmland::E_STATE _eState, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, FARMLAND_WIDTH, FARMLAND_HEIGHT)
{
}

CFarmland::~CFarmland()
{
}

void CFarmland::Ready(void)
{
}

int CFarmland::Update(float _fDeltaTime)
{
	return 0;
}

void CFarmland::LateUpdate(void)
{
}

void CFarmland::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
}

void CFarmland::Release(void)
{
}
