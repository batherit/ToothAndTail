#include "stdafx.h"
#include "CFarmland.h"
#include "CTextureMgr.h"
#include "CPig.h"
#include "CCrop.h"
#include "CGameWorld.h"


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
		// 활성화 애니메이션
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
		if (m_pPig) {
			m_pPig->Cropping(_fDeltaTime);
		}
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
}

void CFarmland::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);					// 농지를 렌더한다.
	for (auto& pCrop : m_vecCrops) {
		pCrop->RegisterToRenderList(_vecRenderList);	// 작물을 렌더한다.
	}
	if (m_pPig) m_pPig->RegisterToRenderList(_vecRenderList);	// 돼지를 렌더한다.
}

void CFarmland::Release(void)
{
	SafelyDeleteObj(m_pPig);
	SafelyDeleteObjs(m_vecCrops);
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
	if (m_pPig) SafelyDeleteObj(m_pPig);
	m_pPig = new CPig(GetGameWorld(), this, GetCommander());
	GetGameWorld().GetListObjs().emplace_back(m_pPig);
}

bool CFarmland::Cropped(float _fCroppedAmount)
{
	return false;
}

void CFarmland::DestroyFarmland(void)
{
}
