#include "stdafx.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CAI_ComState_Idle.h"
#include "CWindmill.h"
#include "CPathGenerator.h"
#include "CCamera.h"


CCommanderAI::CCommanderAI(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CCommander(_rGameWorld, _fX, _fY, _eCommanderType, _clIdentificationTint_ARGB)
{
	SetPrivateCamera(new CCamera(_rGameWorld, this));
	GetPrivateCamera()->SetZoomMultiple(1.3f);
	m_pStateMgr = new CStateMgr<CCommanderAI>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CAI_ComState_Idle(GetGameWorld(), *this));
	m_pPathGenerator = new CPathGenerator(_rGameWorld);
	m_vecWindmills.reserve(10);
	m_vecExtractedWindmills.reserve(10);
}

CCommanderAI::~CCommanderAI()
{
	Release();
}

void CCommanderAI::Ready(void)
{
}

int CCommanderAI::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CCommanderAI::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CCommanderAI::Release(void)
{
	SafelyDeleteObj(m_pStateMgr);
	SafelyDeleteObj(m_pPathGenerator);
	m_vecWindmills.clear();
	m_vecWindmills.shrink_to_fit();
}


vector<CWindmill*>& CCommanderAI::ExtractWindmills(WINDMILL::E_OWN_TYPE _eOwnType)
{
	m_vecExtractedWindmills.clear();
	if (m_vecWindmills.empty() && !DetectWindmills()) return m_vecExtractedWindmills;

	CCommander* pCommander = nullptr;
	for (auto& pWindmill : m_vecWindmills) {
		pCommander = pWindmill->GetCommander();
		switch (_eOwnType)
		{
		case WINDMILL::TYPE_UNOCCUPIED:
			// 점령되지 않은 제분소 추출
			if (!pCommander)
				m_vecExtractedWindmills.emplace_back(pWindmill);
			break;
		case WINDMILL::TYPE_PLAYER:
			// 플레이어 제분소 추출
			if (pCommander && !dynamic_cast<CCommanderAI*>(pCommander))
				m_vecExtractedWindmills.emplace_back(pWindmill);
			break;
		case WINDMILL::TYPE_OWN:
			// 자신의 제분소 추출
			if (pCommander == this) {
				m_vecExtractedWindmills.emplace_back(pWindmill);
			}
			break;
			// 자신 이외의 제분소 추출
		case WINDMILL::TYPE_OTHER:
			if (pCommander && pCommander != this) {
				m_vecExtractedWindmills.emplace_back(pWindmill);
			}
			break;
		case WINDMILL::TYPE_RANDOM:
			// 아무 제분소를 추출
			m_vecExtractedWindmills.emplace_back(pWindmill);
		default:
			break;
		}
	}

	return m_vecExtractedWindmills;
}

bool CCommanderAI::GeneratePathToGoal(const D3DXVECTOR3 & _vGoalPos, CWindmill* _pTargetWindmill)
{
	if (m_pPathGenerator->GeneratePath(GetXY(), _vGoalPos)) {
		// 객체가 어떤 제분소를 기준으로 패스를 작성했는지 알려고 할 때,
		m_pTargetWindmill = _pTargetWindmill;
		return true;
	}
	return false;
}

bool CCommanderAI::MoveAlongPath(float _fDeltaTime)
{
	auto& rPath = m_pPathGenerator->GetPath();
	if (rPath.empty()) return false;
	//m_listPath.back()->SetColor(D3DCOLOR_ARGB(255, 0, 0, 255));
	D3DXVECTOR3 vDir = rPath.front() - GetXY();

	float fLength = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	if ((TILE_HEIGHT >> 1) * BASE_SCALE * 0.5f > fLength)
	{
		rPath.pop_front();
	}

	SetToXY(vDir);
	UpdateSpriteDir();
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

bool CCommanderAI::IsMoving(float & _fToX, float & _fToY)
{
	// 경로가 비어있지 않다면, 이동하고 있는 중이다.
	return !m_pPathGenerator->GetPath().empty();
}

bool CCommanderAI::IsActivating() const
{
	return m_bIsActivating;
}

bool CCommanderAI::IsWavingFlag() const
{
	return false;
}

bool CCommanderAI::DetectWindmills()
{
	CWindmill* pWindmill = nullptr;
	m_vecWindmills.clear();
	// 게임월드 상에 존재하는 제분소를 모두 찾는다.
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pWindmill = dynamic_cast<CWindmill*>(pObj);
		if (pWindmill) {
			m_vecWindmills.emplace_back(pWindmill);
		}
	}

	return !m_vecWindmills.empty();
}
