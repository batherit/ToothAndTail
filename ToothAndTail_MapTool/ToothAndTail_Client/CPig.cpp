#include "stdafx.h"
#include "CPig.h"
#include "CFarmland.h"
#include "CStateMgr.h"
#include "CTextureMgr.h"
#include "CPigState_Patrol.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CUI_UnitHP.h"
#include "CBullet.h"



CPig::CPig(CGameWorld & _rGameWorld, CFarmland* _pFarmland, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f, PIG_WIDTH, PIG_HEIGHT),
	m_pFarmland(_pFarmland)
{
	InitHP(PIG_MAX_HP);
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetDetectionRange(PIG_DETECTION_RANGE);

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
	Release();
}

void CPig::Ready(void)
{
}

int CPig::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	if (m_bIsAttackedRecently) {
		if ((m_fAttackedTickTime -= _fDeltaTime) <= 0.f) {
			m_bIsAttackedRecently = false;
			m_fAttackedTickTime = 0.f;
		}
	}

	return 0;
}

void CPig::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CPig::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}

void CPig::Release(void)
{
	// CComDepObj::Release(); // ���� �Ҹ��ڿ��� ȣ���ϴϱ� ���� ȣ����ص� ��.
	SafelyDeleteObj(m_pStateMgr);
	m_pFarmland = nullptr;
}

void CPig::InvalidateObj(void)
{
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	m_pFarmland->ReleasePig(this);
	m_bIsAttackedRecently = false;
	m_fAttackedTickTime = 0.f;
	CComDepObj::InvalidateObj();
}

void CPig::TakeDamage(float _fDamageAmount, CObj* _pAttacker)
{
	CComDepObj::TakeDamage(_fDamageAmount);
	m_vAttackerPos = dynamic_cast<CBullet*>(_pAttacker)->GetStartPos();
	m_bIsAttackedRecently = true;
	m_fAttackedTickTime = ATTACKED_KEEP_TIME;
}

void CPig::StartPatrol(void)
{
	m_iLocationIndex = rand() % 4;
	
	D3DXVECTOR3 vToDir = m_cvLocationsToPatrol[m_iLocationIndex] - GetXY(CObj::COORD_TYPE_LOCAL);
	D3DXVec3Normalize(&vToDir, &vToDir);

	SetToXY(vToDir.x, vToDir.y);
	
	// ���� ��ȯ
	UpdateSpriteDir();

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