#include "stdafx.h"
#include "CFarmland.h"
#include "CTextureMgr.h"
#include "CPig.h"
#include "CCrop.h"
#include "CGameWorld.h"
#include "CCommander.h"
#include "CWindmill.h"
#include "CWindmillBase.h"


CFarmland::CFarmland(CGameWorld & _rGameWorld, float _fX, float _fY, CFarmland::E_STATE _eState, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, FARMLAND_WIDTH, FARMLAND_HEIGHT),
	m_eState(_eState)
{
	SetRenderLayer(5);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"FARMLAND"));

	switch (_eState) 
	{
	case CFarmland::STATE_UNOCCUPIED: {
		// �ʷϻ� Ǯ���� ���ִ� �ִϸ��̼�
		AnimInfo stAnimInfo(0, 8, 12, 17, 1.5f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		break;
	}
	case CFarmland::STATE_BUILDING: {
		// ���� ���� �ִϸ��̼�
		AnimInfo stAnimInfo(0, 8, 2, 8, 0.8f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		break;
	}
	case CFarmland::STATE_OCCUPIED: {
		AnimInfo stAnimInfo(0, 8, 0, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		GenerateCrops();
		GeneratePig();
		break;
	}
	case CFarmland::STATE_DESTROYED: {
		AnimInfo stAnimInfo(0, 8, 11, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		break;
	}
	}
}

CFarmland::~CFarmland()
{
	Release();
}

void CFarmland::Ready(void)
{
}

int CFarmland::Update(float _fDeltaTime)
{
	switch (m_eState)
	{
	case CFarmland::STATE_UNOCCUPIED: {
		// �ʷϻ� Ǯ���� ���ִ� �ִϸ��̼�
		UpdateAnim(_fDeltaTime);
		break;
	}
	case CFarmland::STATE_BUILDING: {
		// Ȱ��ȭ �ִϸ��̼�
		UpdateAnim(_fDeltaTime);
		if ((m_fElapsedTime += _fDeltaTime) >= FARMLAND_BUILD_SEC) {
			GenerateCrops();
			GeneratePig();
			m_fElapsedTime = 0.f;
			AnimInfo stAnimInfo(0, 8, 0, 1, 1.f, 0, false);
			SetNewAnimInfo(stAnimInfo);
			m_eState = CFarmland::STATE_OCCUPIED;
		}
		break;
	}
	case CFarmland::STATE_OCCUPIED: {
		UpdateAnim(_fDeltaTime);
		break;
	}
	case CFarmland::STATE_DESTROYED: {
		UpdateAnim(_fDeltaTime);
		break;
	}
	}


	return 0;
}

void CFarmland::LateUpdate(void)
{
	CWindmill* pWindmill = dynamic_cast<CWindmill*>(m_pParent);
	if (!pWindmill) return;
	if (WINDMILL::STATE_OCCUPIED != pWindmill->GetState()) return;

	// ������ �� ������ �����̰ų� ���ɵǰ� �ִ� ���¶�� ����.
	if (m_eState == CFarmland::STATE_DESTROYED ||
		m_eState == CFarmland::STATE_BUILDING) return;

	// ��� ���� �۾�
	// ���� ����� �����ϴ���?
	if (!GetCommander()) return;
	// ���� ����� �����ǻ簡 �ִ���?
	if (!GetCommander()->IsSpaceKeyPressed()) return;
	// �� ���� ������ ����� ���Դ���?
	if (!IsPointInTile(GetCommander()->GetXY(), GetXY(), 2.f * TILE_WIDTH * BASE_SCALE, 2.f * TILE_HEIGHT * BASE_SCALE)) return;
	

	switch (m_eState)
	{
	case CFarmland::STATE_UNOCCUPIED:
		// �� ����� ������ ������ �� ���� ������ �ں��� �ִ���?
		if (GetCommander()->GetMoney() < FARMLAND_COST) return;
		// ���� �Ǽ� ����� �����Ѵ�.
		GetCommander()->DecreseMoney(FARMLAND_COST);
		// ���� ���� �ִϸ��̼��� �����Ѵ�.
		SetNewAnimInfo(AnimInfo(0, 8, 2, 8, 0.8f, 0, false));
		m_eState = CFarmland::STATE_BUILDING;
		break;
	case CFarmland::STATE_BUILDING:
		// (������ �ɷ�������,) ���ɵǰ� �ִ� �����̹Ƿ� �Լ� ����
		return;
	case CFarmland::STATE_OCCUPIED:
		// �̹� ������ �ִٸ� �Լ� ����.
		if (m_pPig) return;
		// �� ����� ������ ����� �� ���� ������ �ں��� �ִ���?
		if (GetCommander()->GetMoney() < PIG_COST) return;
		// ����� �����Ѵ�.
		GetCommander()->DecreseMoney(PIG_COST);
		GeneratePig();
		break;

	default:
		break;
	}
}

void CFarmland::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);					// ������ �����Ѵ�.
	for (auto& pCrop : m_vecCrops) {
		pCrop->RegisterToRenderList(_vecRenderList);	// �۹��� �����Ѵ�.
	}
}

void CFarmland::InvalidateObj()
{
	//CComDepObj::InvalidateObj();

	if (m_eState == CFarmland::STATE_BUILDING) {
		// �����ϴ� ���� ��ȿȭ�Ǿ��ٸ�, ���ɵ��� ���� ���·� �����Ѵ�.
		m_eState = CFarmland::STATE_UNOCCUPIED;
		AnimInfo stAnimInfo(0, 8, 12, 17, 1.5f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}

	// � ������� ������ ���� ���·� �����.
	SetCommander(nullptr);

	// ������ ���ش�.
	DO_IF_IS_VALID_OBJ(m_pPig) {
		m_pPig->SetValid(false);
		m_pPig = nullptr;
	}
}

void CFarmland::Release(void)
{
	SafelyDeleteObjs(m_vecCrops);
	DO_IF_IS_VALID_OBJ(m_pPig) {
		m_pPig->SetValid(false);
		m_pPig = nullptr;
	}
}

void CFarmland::GenerateCrops(void)
{
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, -18.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -10.f, -9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 10.f, -9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -20.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 20.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -10.f, 9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 10.f, 9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, 18.f + m_fCropOffsetY, rand() % 4));
}

void CFarmland::GeneratePig(void)
{
	if (m_pPig) return;
	m_pPig = new CPig(GetGameWorld(), this, GetCommander());
	GetGameWorld().GetListObjs().emplace_back(m_pPig);
}

bool CFarmland::Cropped(float _fCroppedAmount)
{
	return false;
}

//void CFarmland::Occupied(CCommander* _pCommander)
//{
//	// ������ ���忡 ������ �ְ�, �� ������ �����Ϸ��� ����� ���� ����� ���ϰ� �ִٸ�,
//	// �̹� �ش� ����� ���ɵ� �����̹Ƿ� ���� �Լ��� �����Ѵ�.
//	if (CFarmland::STATE_OCCUPIED && m_pPig && m_pPig->GetCommander() == _pCommander) return;
//
//	SetCommander(_pCommander);
//	if (CFarmland::STATE_OCCUPIED == m_eState) {
//		DO_IF_IS_VALID_OBJ(m_pPig) {
//			// Ȥ�ö� ���� ������ �ִٸ�, �߸��� �����̹Ƿ� ��ȿȭ ó���� �ϰ� ���ο� ������ �����Ѵ�.
//			m_pPig->SetValid(false);
//			m_pPig = nullptr;
//		}
//		GeneratePig();
//	}
//}
