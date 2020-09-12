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

	//for (auto& pBlockingTile : GetGameWorld().GetMapLoader()->GetBlockingTiles()) {
	//	pBlockingTile->PushOutOfTile(this);
	//}
}

void CCommanderAI::Release(void)
{
	SafelyDeleteObj(m_pStateMgr);
	SafelyDeleteObj(m_pPathGenerator);
	m_vecWindmills.clear();
	m_vecWindmills.shrink_to_fit();
	m_listPath.clear();
}

// �� ���� ã�� �Լ�(AI�� �־� ������ �߿��� �Լ��̴�. '��'!!)
bool CCommanderAI::DetectPlaceToGo()
{
	// 1) �ϴ� ���мҸ� ã�´�. ���ӿ��� �� ���мҰ� �ִٸ�, �� �� �� ���ŵȴ�. // ���� : ���мҴ� �ý��� �� �ı����� �ʴ´�.
	CWindmill* pWindmill = nullptr;
	if (m_vecWindmills.empty()) {
		// ���ӿ��� �� �����ϴ� ���мҸ� ��� ã�´�.
		for (auto& pObj : GetGameWorld().GetListObjs()) {
			pWindmill = dynamic_cast<CWindmill*>(pObj);
			if (pWindmill) {
				m_vecWindmills.emplace_back(pWindmill);
			}
		}
	}

	// ���� ���忡 ���мҰ� ���ٸ� �Լ� ����.
	if (m_vecWindmills.empty()) return false;

	// �湮�� ���� ���мҸ� ã�´�.
	int iIndex = -1;
	do {
		iIndex = rand() % m_vecWindmills.size();
		pWindmill = m_vecWindmills[iIndex];
	} while (m_iOldWindmillIndex == iIndex);
	m_iOldWindmillIndex = iIndex;

	// ���мҸ� �߽����� �湮�� ��ġ�� �����Ѵ�. (���ŷ Ÿ���� ���� ����)
	D3DXVECTOR3 vGoalPos;
	if (!DetectPlaceToGoAroundWindmill(vGoalPos, pWindmill)) return false;

	// ������ġ���� ������ ��ġ���� ��θ� �����Ѵ�.
	m_pPathGenerator->GeneratePath(GetXY(), vGoalPos);
	if (m_pPathGenerator->GetPath().empty()) return false;
	m_listPath.clear();
	m_listPath = m_pPathGenerator->GetPath();

	return true;
}

bool CCommanderAI::DetectPlaceToGoAroundWindmill(D3DXVECTOR3 & _rGoalPos, const CWindmill* _pWindmill)
{
	// �ʺ� �켱 Ž������ ���� Ȯ���ذ��鼭 �� ���� ã�ƺ���.
	bool bVisited[100][100] = { false, };
	queue<POINT> qVisited;	// �湮�� ��带 ����ִ´�.
	D3DXVECTOR3 vStartPos = _pWindmill->GetXY();
	vStartPos.x += (TILE_WIDTH >> 1) * BASE_SCALE;
	CMapLoader* pMapLoader = GetGameWorld().GetMapLoader();
	POINT ptRowColIndexes = pMapLoader->GetRowColIndex(vStartPos);
	qVisited.push(ptRowColIndexes);
	bVisited[ptRowColIndexes.y][ptRowColIndexes.x] = true;

	POINT ptHere;
	POINT ptThere;
	CTile* pTile = nullptr;
	while (!qVisited.empty()) {
		ptHere = qVisited.front();
		qVisited.pop();
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (0 == i && 0 == j) continue; // �ڱ� �ڽ��� ����Ű�Ƿ� ������ �����Ѵ�.
				ptThere.y = ptHere.y + i;
				ptThere.x = ptHere.x + j;
				if (pMapLoader->IsEmptyLot(ptThere, 2, 2, 1, 1)) {
					//���Ͷ��, �̰��� ��ǥ �������� �д�.
					pTile = pMapLoader->GetTile(ptThere.y, ptThere.x);
					_rGoalPos = pTile->GetXY();
					return true;
				}

				// ���͸� ã�� ���ߴٸ�, �̰��� �湮�� ������ ó���ϰ�
				// ���� ��带 ���캻��.
				if (!bVisited[ptThere.y][ptThere.x]) {
					qVisited.push(ptThere);
					bVisited[ptThere.y][ptThere.x] = true;
				}
			}
		}
	}

	_rGoalPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	return false;
}

bool CCommanderAI::MoveAlongPath(float _fDeltaTime)
{
	if (m_listPath.empty()) return false;
	//m_listPath.back()->SetColor(D3DCOLOR_ARGB(255, 0, 0, 255));
	D3DXVECTOR3 vDir = m_listPath.front()->GetXY() - GetXY();

	float fDist = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	if (5.f > fDist)
	{
		m_listPath.pop_front();
	}

	SetToXY(vDir);
	UpdateSpriteDir();
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

bool CCommanderAI::IsMoving(float & _fToX, float & _fToY)
{
	// ��ΰ� ������� �ʴٸ�, �̵��ϰ� �ִ� ���̴�.
	return !m_listPath.empty();
}

bool CCommanderAI::IsBuilding() const
{
	return false;
}

bool CCommanderAI::IsOccupying() const
{
	return false;
}
