#include "stdafx.h"
#include "CWindmillBase.h"
#include "CTurbine.h"
#include "CTextureMgr.h"


CWindmillBase::CWindmillBase(CGameWorld & _rGameWorld, float _fX, float _fY, WINDMILL::E_STATE _eState, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT),
	m_eState(_eState)
{
	SetRenderLayer(10);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_BASE"));
	GenerateIdentificationTintObj(WINDMILL_WIDTH, WINDMILL_HEIGHT, L"WINDMILL_BASE_TINT");

	switch (_eState)
	{
	case WINDMILL::STATE_UNOCCUPIED:
	{
		AnimInfo stAnimInfo(0, 16, 0, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	case WINDMILL::STATE_BUILDING:
	{
		AnimInfo stAnimInfo(0, 16, 0, 97, 7.f, 1, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	case WINDMILL::STATE_OCCUPIED:
	{
		m_pTurbine = new CTurbine(_rGameWorld, 0.f, 0.f, CTurbine::STATE_COMPLETED, _pCommander);
		m_pTurbine->SetParent(this);
		AnimInfo stAnimInfo(0, 16, 203, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	}
}

CWindmillBase::~CWindmillBase()
{
	Release();
}

void CWindmillBase::Ready(void)
{
}

int CWindmillBase::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case WINDMILL::STATE_BUILDING:
		if (1 == UpdateAnim(_fDeltaTime)) {
			// 베이스 건설이 완료된 이후
			if (!m_pTurbine) {
				m_pTurbine = new CTurbine(GetGameWorld(), 0.f, 0.f, CTurbine::STATE_BUILDING, GetCommander());
				m_pTurbine->SetParent(this);
			}
			else if(1 == m_pTurbine->Update(_fDeltaTime)) {
				m_eState = WINDMILL::STATE_OCCUPIED;
				AnimInfo stAnimInfo(0, 16, 203, 1, 1.f, 0, false);
				SetNewAnimInfo(stAnimInfo);
			}
		}
		break;
	case WINDMILL::STATE_OCCUPIED:
		UpdateAnim(_fDeltaTime);
		m_pTurbine->Update(_fDeltaTime);
		break;
	}

	return 0;
}

void CWindmillBase::LateUpdate(void)
{
}

void CWindmillBase::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);
	if (m_pTurbine) _vecRenderList.emplace_back(m_pTurbine);
}

void CWindmillBase::Release(void)
{
	SafelyDeleteObj(m_pTurbine);
}

void CWindmillBase::SetWindmillBaseState(WINDMILL::E_STATE _eState)
{
	if (m_eState == _eState) return;

	Release();
	m_eState = _eState;
	switch (_eState)
	{
	case WINDMILL::STATE_UNOCCUPIED:
	{
		AnimInfo stAnimInfo(0, 16, 0, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	case WINDMILL::STATE_BUILDING:
	{
		AnimInfo stAnimInfo(0, 16, 0, 97, 7.f, 1, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	case WINDMILL::STATE_OCCUPIED:
	{
		m_pTurbine = new CTurbine(GetGameWorld(), 0.f, 0.f, CTurbine::STATE_COMPLETED, GetCommander());
		m_pTurbine->SetParent(this);
		AnimInfo stAnimInfo(0, 16, 203, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}
	break;
	case WINDMILL::STATE_DESTROYED:
	{
		AnimInfo stAnimInfo(0, 16, 204, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		break;
	}
	}
}
