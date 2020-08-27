#include "stdafx.h"
#include "CTurbine.h"
#include "CTextureMgr.h"
#include "CCommander.h"



CTurbine::CTurbine(CGameWorld & _rGameWorld, float _fX, float _fY, CTurbine::E_STATE _eState, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, TURBINE_WIDTH, TURBINE_HEIGHT),
	m_eState(_eState)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_TURBINE"));

	// 색상 객체를 생성한다.
	GenerateIdentificationTintObj(TURBINE_WIDTH, TURBINE_HEIGHT, L"WINDMILL_TURBINE_TINT");

	switch (_eState)
	{
	case CTurbine::STATE_BUILDING:
	{
		AnimInfo stAnimInfo(0, 16, 0, 45, 3.0f, 1, false);
		SetNewAnimInfo(stAnimInfo);
	}
		break;
	case CTurbine::STATE_COMPLETED: 
	{
		AnimInfo stAnimInfo(0, 16, 45, 60, 5.0f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}	
		break;
	default:
		break;
	}

	
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
		if (1 == UpdateAnim(_fDeltaTime)) {
			AnimInfo stAnimInfo(0, 16, 45, 60, 5.0f, 0, false);
			SetNewAnimInfo(stAnimInfo);
			m_eState = CTurbine::STATE_COMPLETED;
		}
		break;
	case CTurbine::STATE_COMPLETED:
		UpdateAnim(_fDeltaTime);
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

