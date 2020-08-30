#include "stdafx.h"
#include "CPig.h"
#include "CFarmland.h"
#include "CStateMgr.h"
#include "CTextureMgr.h"
#include "CPigState_Patrol.h"



CPig::CPig(CGameWorld & _rGameWorld, CFarmland* _pFarmland, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f, PIG_WIDTH, PIG_HEIGHT),
	m_pFarmland(_pFarmland)
{
	SetRenderLayer(10);
	SetShadow(true);
	SetParent(_pFarmland);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"PIG"));
	GenerateIdentificationTintObj(PIG_WIDTH, PIG_HEIGHT, L"PIG_TINT");

	m_pStateMgr = new CStateMgr<CPig>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CPigState_Patrol(GetGameWorld(), *this));
}

CPig::~CPig()
{
}

void CPig::Ready(void)
{
}

int CPig::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CPig::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CPig::Release(void)
{
	SafelyDeleteObj(m_pStateMgr);
	m_pFarmland = nullptr;
}

bool CPig::Cropping(float _fDeltaTime)
{
	return true;
}

void CPig::StartPatrol(void)
{
	m_iLocationIndex = rand() % 4;
	
	D3DXVECTOR3 vToDir = m_cvLocationsToPatrol[m_iLocationIndex] - GetXY(CObj::COORD_TYPE_LOCAL);
	D3DXVec3Normalize(&vToDir, &vToDir);

	SetToXY(vToDir.x, vToDir.y);
	
	if (vToDir.y < 0.f) SetScaleX(fabs(GetScaleX()));
	if (vToDir.y > 0.f) SetScaleX(-fabs(GetScaleX()));
	if (vToDir.x > 0.f) SetScaleX(fabs(GetScaleX()));
	if (vToDir.x < 0.f) SetScaleX(-fabs(GetScaleX()));

	SetSpeed(PIG_SPEED);
}

int CPig::UpdatePatrol(float _fDeltaTime)
{
	D3DXVECTOR3 vToDir = m_cvLocationsToPatrol[m_iLocationIndex] - GetXY(CObj::COORD_TYPE_LOCAL);
	
	float _fLength = D3DXVec3Length(&vToDir);
	if (_fLength <= 1.f) {
		return 1;
	}

	D3DXVec3Normalize(&vToDir, &vToDir);
	SetToXY(vToDir.x, vToDir.y);
	MoveByDeltaTime(_fDeltaTime);
	return 0;
}