#include "stdafx.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CAI_ComState_Idle.h"
#include "CWindmill.h"
#include "CPathGenerator.h"


CCommanderAI::CCommanderAI(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CCommander(_rGameWorld, _fX, _fY, _eCommanderType, _clIdentificationTint_ARGB)
{
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

//CWindmill * CCommanderAI::ExtractWindmill(WINDMILL::E_OWN_TYPE _eOwnType)
//{
//	
//}

// �� ���� ã�� �Լ�(AI�� �־� ������ �߿��� �Լ��̴�. '��'!!)
//bool CCommanderAI::DetectPlaceToGo()
//{
//	// 1) �ϴ� ���мҸ� ã�´�. ���ӿ��� �� ���мҰ� �ִٸ�, �� �� �� ���ŵȴ�. // ���� : ���мҴ� �ý��� �� �ı����� �ʴ´�.
//	if (m_vecWindmills.empty() && !DetectWindmills()) return false;
//
//	// �湮�� ���� ���мҸ� ã�´�.
//	CWindmill* pWindmill;
//	int iIndex = -1;
//	do {
//		iIndex = rand() % m_vecWindmills.size();
//		pWindmill = m_vecWindmills[iIndex];
//	} while (m_pDetectedWindmill == pWindmill);
//	//m_iOldWindmillIndex = iIndex;
//	m_pDetectedWindmill = pWindmill;
//
//	// ���мҸ� �߽����� �湮�� ��ġ�� �����Ѵ�. (���ŷ Ÿ���� ���� ����)
//	D3DXVECTOR3 vGoalPos;
//	if (!DetectEmptyLotToGoAroundWindmill(vGoalPos, pWindmill)) return false;
//
//	// ������ġ���� ������ ��ġ���� ��θ� �����Ѵ�.
//	m_pPathGenerator->GeneratePath(GetXY(), vGoalPos);
//	if (m_pPathGenerator->GetPath().empty()) return false;
//
//	return true;
//}

//bool CCommanderAI::DetectEmptyLotToGoAroundWindmill(D3DXVECTOR3 & _rGoalPos, const CWindmill* _pWindmill)
//{
//	// �ʺ� �켱 Ž������ ���� Ȯ���ذ��鼭 �� ���� ã�ƺ���.
//	bool bVisited[100][100] = { false, };
//	queue<POINT> qVisited;	// �湮�� ��带 ����ִ´�.
//	D3DXVECTOR3 vStartPos = _pWindmill->GetXY();
//	vStartPos.x += (TILE_WIDTH >> 1) * BASE_SCALE;
//	CMapLoader* pMapLoader = GetGameWorld().GetMapLoader();
//	POINT ptRowColIndexes = pMapLoader->GetRowColIndex(vStartPos);
//	qVisited.push(ptRowColIndexes);
//	bVisited[ptRowColIndexes.y][ptRowColIndexes.x] = true;
//
//	POINT ptHere;
//	POINT ptThere;
//	CTile* pTile = nullptr;
//	while (!qVisited.empty()) {
//		ptHere = qVisited.front();
//		qVisited.pop();
//		for (int i = -1; i <= 1; ++i) {
//			for (int j = -1; j <= 1; ++j) {
//				if (0 == i && 0 == j) continue; // �ڱ� �ڽ��� ����Ű�Ƿ� ������ �����Ѵ�.
//				ptThere.y = ptHere.y + i;
//				ptThere.x = ptHere.x + j;
//				if (pMapLoader->IsEmptyLot(ptThere, 2, 2, 1, 1)) {
//					//���Ͷ��, �̰��� ��ǥ �������� �д�.
//					pTile = pMapLoader->GetTile(ptThere.y, ptThere.x);
//					_rGoalPos = pTile->GetXY();
//					return true;
//				}
//
//				// ���͸� ã�� ���ߴٸ�, �̰��� �湮�� ������ ó���ϰ�
//				// ���� ��带 ���캻��.
//				if (!bVisited[ptThere.y][ptThere.x]) {
//					qVisited.push(ptThere);
//					bVisited[ptThere.y][ptThere.x] = true;
//				}
//			}
//		}
//	}
//
//	_rGoalPos = D3DXVECTOR3(0.f, 0.f, 0.f);
//	return false;
//}

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
			// ���ɵ��� ���� ���м� ����
			if (!pCommander)
				m_vecExtractedWindmills.emplace_back(pWindmill);
			break;
		case WINDMILL::TYPE_PLAYER:
			// �÷��̾� ���м� ����
			if (pCommander && !dynamic_cast<CCommanderAI*>(pCommander))
				m_vecExtractedWindmills.emplace_back(pWindmill);
			break;
		case WINDMILL::TYPE_OWN:
			// �ڽ��� ���м� ����
			if (pCommander == this) {
				m_vecExtractedWindmills.emplace_back(pWindmill);
			}
			break;
			// �ڽ� �̿��� ���м� ����
		case WINDMILL::TYPE_OTHER:
			if (!pCommander && pCommander != this) {
				m_vecExtractedWindmills.emplace_back(pWindmill);
			}
			break;
		case WINDMILL::TYPE_RANDOM:
			// �ƹ� ���мҸ� ����
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
		// ��ü�� � ���мҸ� �������� �н��� �ۼ��ߴ��� �˷��� �� ��,
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
	// ��ΰ� ������� �ʴٸ�, �̵��ϰ� �ִ� ���̴�.
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
	// ���ӿ��� �� �����ϴ� ���мҸ� ��� ã�´�.
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pWindmill = dynamic_cast<CWindmill*>(pObj);
		if (pWindmill) {
			m_vecWindmills.emplace_back(pWindmill);
		}
	}

	return !m_vecWindmills.empty();
}
