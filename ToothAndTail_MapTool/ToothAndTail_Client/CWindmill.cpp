#include "stdafx.h"
#include "CWindmill.h"
#include "CTextureMgr.h"
#include "CCommander.h"
#include "CWindmillBase.h"
#include "CTurbine.h"


CWindmill::CWindmill(CGameWorld & _rGameWorld, float _fX, float _fY, CWindmill::E_STATE _eState, CCommander* _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT),
	m_eState(_eState)
{
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
	m_pGround->SetParent(this);
	m_pGround->SetRenderLayer(0);

	switch (_eState)
	{
	case CWindmill::STATE_UNOCCUPIED:
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_UNOCCUPIED, _pCommander);
		break;
	case CWindmill::STATE_BUILDING: 
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_BUILDING, _pCommander);
		break;
	case CWindmill::STATE_OCCUPIED:
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_OCCUPIED, _pCommander);
		break;
	default:
		break;
	}

	if (m_pWindmillBase) m_pWindmillBase->SetParent(this);
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
	if (m_pWindmillBase) m_pWindmillBase->Update(_fDeltaTime);

	return 0;
}

void CWindmill::LateUpdate(void)
{
}

void CWindmill::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	// TODO : 자신을 그리고 싶으면 _vecRenderList.emplace_back(this);를 추가합니다.
	if (m_pGround) m_pGround->RegisterToRenderList(_vecRenderList);
	if (m_pWindmillBase) m_pWindmillBase->RegisterToRenderList(_vecRenderList);
}

void CWindmill::Release(void)
{
	SafelyDeleteObj(m_pWindmillBase);
	SafelyDeleteObj(m_pGround);
}

CObj * CWindmill::GetCollider(void)
{
	return m_pWindmillBase;
}
