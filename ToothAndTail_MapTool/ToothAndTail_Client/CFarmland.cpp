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
		// 초록색 풀들이 나있는 애니메이션
		AnimInfo stAnimInfo(0, 8, 12, 17, 1.5f, 0, false);
		SetNewAnimInfo(stAnimInfo);
		break;
	}
	case CFarmland::STATE_BUILDING: {
		// 농장 개간 애니메이션
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
		// 초록색 풀들이 나있는 애니메이션
		UpdateAnim(_fDeltaTime);
		break;
	}
	case CFarmland::STATE_BUILDING: {
		// 활성화 애니메이션
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

	// 지력이 다 떨어진 상태이거나 점령되고 있는 상태라면 종료.
	if (m_eState == CFarmland::STATE_DESTROYED ||
		m_eState == CFarmland::STATE_BUILDING) return;

	// 기수 점령 작업
	// 점령 기수가 존재하는지?
	if (!GetCommander()) return;
	// 점령 기수가 구매의사가 있는지?
	if (!GetCommander()->IsSpaceKeyPressed()) return;
	// 이 농장 부지에 기수가 들어왔는지?
	if (!IsPointInTile(GetCommander()->GetXY(), GetXY(), 2.f * TILE_WIDTH * BASE_SCALE, 2.f * TILE_HEIGHT * BASE_SCALE)) return;
	

	switch (m_eState)
	{
	case CFarmland::STATE_UNOCCUPIED:
		// 그 기수가 농장을 점령할 수 있을 정도의 자본이 있는지?
		if (GetCommander()->GetMoney() < FARMLAND_COST) return;
		// 농장 건설 비용을 차감한다.
		GetCommander()->DecreseMoney(FARMLAND_COST);
		// 농장 개간 애니메이션을 세팅한다.
		SetNewAnimInfo(AnimInfo(0, 8, 2, 8, 0.8f, 0, false));
		m_eState = CFarmland::STATE_BUILDING;
		break;
	case CFarmland::STATE_BUILDING:
		// (위에서 걸러지지만,) 점령되고 있는 상태이므로 함수 종료
		return;
	case CFarmland::STATE_OCCUPIED:
		// 이미 돼지가 있다면 함수 종료.
		if (m_pPig) return;
		// 그 기수가 돼지를 고용할 수 있을 정도의 자본이 있는지?
		if (GetCommander()->GetMoney() < PIG_COST) return;
		// 비용을 차감한다.
		GetCommander()->DecreseMoney(PIG_COST);
		GeneratePig();
		break;

	default:
		break;
	}
}

void CFarmland::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);					// 농지를 렌더한다.
	for (auto& pCrop : m_vecCrops) {
		pCrop->RegisterToRenderList(_vecRenderList);	// 작물을 렌더한다.
	}
}

void CFarmland::InvalidateObj()
{
	//CComDepObj::InvalidateObj();

	if (m_eState == CFarmland::STATE_BUILDING) {
		// 개간하는 도중 무효화되었다면, 점령되지 않은 상태로 세팅한다.
		m_eState = CFarmland::STATE_UNOCCUPIED;
		AnimInfo stAnimInfo(0, 8, 12, 17, 1.5f, 0, false);
		SetNewAnimInfo(stAnimInfo);
	}

	// 어떤 기수에도 속하지 않은 상태로 만든다.
	SetCommander(nullptr);

	// 돼지를 없앤다.
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
//	// 개간된 농장에 돼지가 있고, 그 돼지가 점령하려는 기수와 같은 기수를 지니고 있다면,
//	// 이미 해당 기수에 점령된 상태이므로 점령 함수를 종료한다.
//	if (CFarmland::STATE_OCCUPIED && m_pPig && m_pPig->GetCommander() == _pCommander) return;
//
//	SetCommander(_pCommander);
//	if (CFarmland::STATE_OCCUPIED == m_eState) {
//		DO_IF_IS_VALID_OBJ(m_pPig) {
//			// 혹시라도 만약 돼지가 있다면, 잘못된 돼지이므로 무효화 처리를 하고 새로운 돼지를 생성한다.
//			m_pPig->SetValid(false);
//			m_pPig = nullptr;
//		}
//		GeneratePig();
//	}
//}
