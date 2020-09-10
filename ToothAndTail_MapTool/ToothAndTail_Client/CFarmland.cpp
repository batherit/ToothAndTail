#include "stdafx.h"
#include "CFarmland.h"
#include "CTextureMgr.h"
#include "CPig.h"
#include "CCrop.h"
#include "CGameWorld.h"
#include "CCommander.h"
#include "CWindmill.h"
#include "CWindmillBase.h"
#include "CUI_BuildGauge.h"


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
		m_iCurrentFertility = FARMLAND_FERTILITY;
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

	m_pBuildGauge = new CUI_BuildGauge(_rGameWorld, this, UNIT::TYPE_PIG);
	m_pBuildGauge->SetY(-15.f);
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
		m_fElapsedTime += _fDeltaTime;
		m_pBuildGauge->UpdateGauge(m_fElapsedTime / FARMLAND_BUILD_SEC);
		if (m_fElapsedTime >= FARMLAND_BUILD_SEC) {
			m_iCurrentFertility = FARMLAND_FERTILITY; // 농장 지력 설정
			GenerateCrops();
			GeneratePig();
			m_fElapsedTime = 0.f;
			AnimInfo stAnimInfo(0, 8, 0, 1, 1.f, 0, false);
			SetNewAnimInfo(stAnimInfo);
			m_pBuildGauge->UpdateGauge(1.f);
			m_eState = CFarmland::STATE_OCCUPIED;
		}

		break;
	}
	case CFarmland::STATE_OCCUPIED: {
		UpdateAnim(_fDeltaTime);
		if (m_bIsPigGenerating) {
			m_fElapsedTime += _fDeltaTime;
			m_pBuildGauge->UpdateGauge(m_fElapsedTime / PIG_GEN_SEC);
			if (m_fElapsedTime >= PIG_GEN_SEC) {
				GeneratePig();
				m_fElapsedTime = 0.f;
				m_pBuildGauge->UpdateGauge(1.f);
				m_bIsPigGenerating = false;
			}
		}
		break;
	}
	case CFarmland::STATE_DESTROYED: {
		//UpdateAnim(_fDeltaTime);
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
		// 이미 돼지가 있거나, 돼지를 생성 중이라면 함수 종료.
		if (m_pPig || m_bIsPigGenerating) return;
		// 그 기수가 돼지를 고용할 수 있을 정도의 자본이 있는지?
		if (GetCommander()->GetMoney() < PIG_COST) return;
		// 비용을 차감한다.
		GetCommander()->DecreseMoney(PIG_COST);
		// 돼지 생성 상태로 만든다.
		m_bIsPigGenerating = true;
		m_fElapsedTime = 0.f;
		break;

	default:
		break;
	}
}

void CFarmland::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);					// 농지를 렌더한다.
	for (auto& pCrop : m_vecCrops) {
		pCrop->RegisterToRenderList(_vecRenderList);			// 작물을 렌더한다.
	}
	if (m_eState == CFarmland::STATE_BUILDING || m_bIsPigGenerating) {
		m_pBuildGauge->RegisterToRenderList(_vecRenderList);
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
		// 무효화한다.
		m_pPig->InvalidateObj();
		m_pPig = nullptr;
	}

	// 개간 도중 파괴되었으면 걸린 시간을 0으로 초기화해주어야
	// 새로이 개간할 때 문제가 없다.(== 0부터 시작한다는 뜻)
	m_fElapsedTime = 0.f;
	m_bIsPigGenerating = false;
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
	// 한 번 생성되고 나서는 다시 생성될 일이 없다.
	if (m_bIsCropGenerated) return;

	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, -18.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -10.f, -9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 10.f, -9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -20.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 20.f, 0.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, -10.f, 9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 10.f, 9.f + m_fCropOffsetY, rand() % 4));
	m_vecCrops.emplace_back(new CCrop(GetGameWorld(), this, 0.f, 18.f + m_fCropOffsetY, rand() % 4));
	m_bIsCropGenerated = true;
}

void CFarmland::GeneratePig(void)
{
	if (m_pPig) return;
	m_pPig = new CPig(GetGameWorld(), this, GetCommander());
	GetGameWorld().GetListObjs().emplace_back(m_pPig);
}

int CFarmland::Cropped(void)
{
	// 점령된 상태인지?
	if (m_eState != CFarmland::STATE_OCCUPIED) return 0;
	// 비옥도가 0보다 큰지?
	if (m_iCurrentFertility <= 0) return 0;
	// 돼지가 존재하는지?
	if (!m_pPig) return 0;
	// 점령된 상태o, 비옥함o, 돼지존재o

	// 비옥도(수확가능량과 같다.)를 1감소시킨다.
	m_iCurrentFertility -= PIG_CROP_AMOUNT;

	if (m_iCurrentFertility <= 0) {
		// 비옥도가 0이 되었다는 것은 더이상 농장 기능을 못한다는 것이다.
		// 남아있는 작물들을 모두 없앤다.
		SafelyDeleteObjs(m_vecCrops);
		// 돼지가 있다면 없앤다.
		DO_IF_IS_VALID_OBJ(m_pPig) {
			// 무효화한다.
			m_pPig->InvalidateObj();
			m_pPig = nullptr;
		}
		// 파괴된 농장 스프라이트를 세팅한다.
		AnimInfo stAnimInfo(0, 8, 11, 1, 1.f, 0, false);
		SetNewAnimInfo(stAnimInfo);

		// 파괴된 상태로 세팅한다/
		m_eState = CFarmland::STATE_DESTROYED;

		// 작물은 수확했으므로 1반환
		return PIG_CROP_AMOUNT - m_iCurrentFertility;
	}

	int iAmountPerCrop = FARMLAND_FERTILITY / 9;			// 작물 하나에서 얻을 수 있는 수확물량
	int iCropNum = m_iCurrentFertility / iAmountPerCrop + 1;	// 현재 작물수
	
	int iIndexToRemove = -1;
	while (m_vecCrops.size() > iCropNum) {
		iIndexToRemove = rand() % m_vecCrops.size();
		m_vecCrops.erase(m_vecCrops.begin() + iIndexToRemove);
	}
	return PIG_CROP_AMOUNT;
}

void CFarmland::ReleasePig(CPig * _pPig)
{
	if (m_pPig != _pPig) return;
	m_pPig = nullptr;
}

void CFarmland::SetCommander(CCommander * _pCommander, D3DXCOLOR _clIdentificationTint)
{
	CComDepObj::SetCommander(_pCommander, _clIdentificationTint);
	m_pBuildGauge->UpdateRenderColor();
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
