#include "stdafx.h"
#include "CTurbine.h"
#include "CTextureMgr.h"
#include "CCommander.h"



CTurbine::CTurbine(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, TURBINE_WIDTH, TURBINE_HEIGHT),
	m_eState(CTurbine::STATE_BUILDING)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_TURBINE"));
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	GenerateIdentificationTintObj(TURBINE_WIDTH, TURBINE_HEIGHT, L"WINDMILL_TURBINE_TINT");

	AnimInfo stAnimInfo(0, 16, 0, 45, 1.5f, 1, false);
	SetNewAnimInfo(stAnimInfo);
}

CTurbine::~CTurbine()
{
}

void CTurbine::Ready(void)
{
}

int CTurbine::Update(float _fDeltaTime)
{
	switch (m_eState)
	{
	case CTurbine::STATE_BUILDING:
		break;
	case CTurbine::STATE_COMPLETED:
		break;
	default:
		break;
	}

	return 0;
}

void CTurbine::LateUpdate(void)
{
}

void CTurbine::Release(void)
{
}

