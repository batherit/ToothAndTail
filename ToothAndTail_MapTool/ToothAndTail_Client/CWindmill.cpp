#include "stdafx.h"
#include "CWindmill.h"
#include "CTextureMgr.h"
#include "CCommander.h"
#include "CTurbine.h"


CWindmill::CWindmill(CGameWorld & _rGameWorld, float _fX, float _fY, CWindmill::E_STATE _eState, CCommander* _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT),
	m_eState(_eState)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_BASE"));
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	GenerateIdentificationTintObj(WINDMILL_WIDTH, WINDMILL_HEIGHT, L"WINDMILL_BASE_TINT");
	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 50.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
	m_pGround->SetParent(this);

	switch (_eState)
	{
	case CWindmill::STATE_UNOCCUPIED:
	{
		AnimInfo stAnimInfo(0, 16, 0, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
		break;
	case CWindmill::STATE_BUILDING: 
	{
		AnimInfo stAnimInfo(0, 16, 0, 97, 7.f, 1, false);
		SetNewAnimInfo(stAnimInfo);
	}
		break;
	case CWindmill::STATE_OCCUPIED:
	{
		m_pTurbine = new CTurbine(_rGameWorld, 0.f, 0.f, CTurbine::STATE_COMPLETED, _pCommander);
		m_pTurbine->SetParent(this);
		AnimInfo stAnimInfo(0, 16, 203, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
		break;
	default:
		break;
	}
}


CWindmill::~CWindmill()
{
	Release();
}

void CWindmill::Ready(void)
{
}

int CWindmill::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case CWindmill::STATE_UNOCCUPIED:
		break;
	case CWindmill::STATE_BUILDING:
		if (1 == UpdateAnim(_fDeltaTime)) {
			m_pTurbine = new CTurbine(GetGameWorld(), 0.f, 0.f, CTurbine::STATE_BUILDING, GetCommander());
			m_pTurbine->SetParent(this);
			AnimInfo stAnimInfo(0, 16, 203, 1, 1.f, 0, false);
			SetNewAnimInfo(stAnimInfo);
			m_eState = CWindmill::STATE_OCCUPIED;
		}
		break;
	case CWindmill::STATE_OCCUPIED:
		UpdateAnim(_fDeltaTime);
		m_pTurbine->Update(_fDeltaTime);
		break;
	}

	return 0;
}

void CWindmill::LateUpdate(void)
{
}

void CWindmill::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	if (m_pGround) _vecRenderList.emplace_back(m_pGround);
	_vecRenderList.emplace_back(this);
	if (m_pTurbine) _vecRenderList.emplace_back(m_pTurbine);
}

void CWindmill::Release(void)
{
	SafelyDeleteObj(m_pTurbine);
	SafelyDeleteObj(m_pGround);
}
