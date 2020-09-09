#include "stdafx.h"
#include "CWindmill.h"
#include "CTextureMgr.h"
#include "CCommander.h"
#include "CWindmillBase.h"
#include "CTurbine.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CFarmland.h"
#include "CBurst.h"
#include "CUI_UnitHP.h"


//CWindmill::CWindmill(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander* _pCommander)
//	:
//	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT)
//{
//	// 안쓰이는 생성자,,,
//	SetScaleXY(BASE_SCALE, BASE_SCALE);
//
//	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
//	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
//	m_pGround->SetParent(this);
//	m_pGround->SetRenderLayer(0);
//
//	/*m_eState = CWindmill::STATE_UNOCCUPIED;
//	if (_pCommander) m_eState = CWindmill::STATE_OCCUPIED;*/
//
//	switch (m_eState)
//	{
//	case CWindmill::STATE_UNOCCUPIED:
//	{
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_UNOCCUPIED, _pCommander);
//		D3DXVECTOR3 vFarmlandPos;
//		CFarmland* pFarmland = nullptr;
//		for (int i = 0; i < 3; ++i) {
//			for (int j = 0; j < 3; ++j) {
//				if (i == 1 && j == 1) continue;
//				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH >> 1);
//				vFarmlandPos.y = (i - j) * (TILE_HEIGHT >> 1);
//				vFarmlandPos.z = 0.f;
//				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, CFarmland::STATE_UNOCCUPIED, GetCommander());
//				m_vecFarmlands.emplace_back(pFarmland);
//			}
//		}
//		break;
//	}
//	case CWindmill::STATE_BUILDING: 
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_BUILDING, _pCommander);
//		break;
//	case CWindmill::STATE_OCCUPIED:
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_OCCUPIED, _pCommander);
//		break;
//	default:
//		break;
//	}
//
//	if (m_pWindmillBase) m_pWindmillBase->SetParent(this);
//}

CWindmill::CWindmill(CGameWorld & _rGameWorld, int _iLineIndex, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f, WINDMILL_WIDTH, WINDMILL_HEIGHT)
{
	GetUIUnitHP()->SetY(-20.f);
	SetMinimapSign(MINIMAP::SIGN_WINDMILL);
	// 이부분 마음에 안들어
	SetTileSiteInfo(TileSiteInfo(_iLineIndex, 6, 6, 3, 3));
	CMapLoader* pMapLoader = _rGameWorld.GetMapLoader();
	pMapLoader->SetSiteType(TileSiteInfo(_iLineIndex, 6, 6, 3, 3), TILE::TYPE_NO);
	pMapLoader->SetSiteType(TileSiteInfo(_iLineIndex, 2, 2, 1, 1), TILE::TYPE_BLOCKING);
	pMapLoader->UpdateBlockingTiles();
	SetXY(pMapLoader->GetSiteCenter(TileSiteInfo(_iLineIndex, 2, 2, 1, 1)));
	/*D3DXVECTOR3 vTilePos = pMapLoader->GetTile(_iLineIndex)->GetXY();
	vTilePos.x -= (TILE_WIDTH >> 1) * BASE_SCALE;
	SetXY(vTilePos);*/
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	// 그라운드 생성
	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
	m_pGround->SetParent(this);
	m_pGround->SetRenderLayer(0);

	// 상태 설정에 따른 농장 생성
	/*m_eState = CWindmill::STATE_UNOCCUPIED;
	if (_pCommander) m_eState = CWindmill::STATE_OCCUPIED;*/

	if (_pCommander) {
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, WINDMILL::STATE_OCCUPIED, _pCommander);
		m_pWindmillBase->SetParent(this);
		D3DXVECTOR3 vFarmlandPos;
		CFarmland* pFarmland = nullptr;
		CFarmland::E_STATE eFarmlandStates[9] = {
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_OCCUPIED, CFarmland::STATE_OCCUPIED,
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_END, CFarmland::STATE_OCCUPIED,
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED
		};
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH);
				vFarmlandPos.y = (i - j) * (TILE_HEIGHT);
				vFarmlandPos.z = 0.f;
				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, eFarmlandStates[i * 3 + j], _pCommander);
				pFarmland->SetParent(this);
				m_vecFarmlands.emplace_back(pFarmland);
			}
		}
	}
	else {
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, WINDMILL::STATE_UNOCCUPIED, _pCommander);
		m_pWindmillBase->SetParent(this);
		D3DXVECTOR3 vFarmlandPos;
		CFarmland* pFarmland = nullptr;
		CFarmland::E_STATE eFarmlandStates[9] = {
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED,
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_END, CFarmland::STATE_UNOCCUPIED,
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED
		};
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH);
				vFarmlandPos.y = (i - j) * (TILE_HEIGHT);
				vFarmlandPos.z = 0.f;
				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, eFarmlandStates[i * 3 + j], _pCommander);
				pFarmland->SetParent(this);
				m_vecFarmlands.emplace_back(pFarmland);
			}
		}
	}	
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
	GetUIUnitHP()->Update(_fDeltaTime);
	if (m_pWindmillBase) m_pWindmillBase->Update(_fDeltaTime);
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->Update(_fDeltaTime);
	}

	return 0;
}

void CWindmill::LateUpdate(void)
{
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->LateUpdate();
	}
}

void CWindmill::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	// TODO : 자신을 그리고 싶으면 CObj::RegisterToRenderList(_vecRenderList)를 추가합니다.
	if (m_pGround) m_pGround->RegisterToRenderList(_vecRenderList);
	if (m_pWindmillBase) m_pWindmillBase->RegisterToRenderList(_vecRenderList);
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->RegisterToRenderList(_vecRenderList);
	}
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}

void CWindmill::Release(void)
{
	SafelyDeleteObj(m_pWindmillBase);
	SafelyDeleteObj(m_pGround);
	SafelyDeleteObjs(m_vecFarmlands);
}

void CWindmill::InvalidateObj()
{
	// 일종의 무효화처리(무효화된 상태이므로 더이상 무효화하지 마라.)
	if (GetState() == WINDMILL::STATE_DESTROYED) return;
	// 자기 자신을 무효화한다.
	//CComDepObj::InvalidateObj();
	SetCommander(nullptr);
	D3DXVECTOR3 vBurstPos = GetXY();
	vBurstPos.y -= 35.f * BASE_SCALE;
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), vBurstPos, 1.2f, 3.f));
	// TODO : 점령 과정을 작성한다.
	if (m_pWindmillBase) {
		m_pWindmillBase->SetCommander(nullptr);
		m_pWindmillBase->SetWindmillBaseState(WINDMILL::STATE_DESTROYED);
	}
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->InvalidateObj();
	}
}

void CWindmill::Occupied(CCommander* _pCommander)
{
	/*if (m_eState != CWindmill::STATE_UNOCCUPIED && m_eState != CWindmill::STATE_DESTROYED) return;
	m_eState = CWindmill::STATE_BUILDING;*/
	SetCommander(_pCommander);
	// TODO : 점령 과정을 작성한다.
	if (m_pWindmillBase) {
		m_pWindmillBase->SetCommander(_pCommander);
		m_pWindmillBase->SetWindmillBaseState(WINDMILL::STATE_BUILDING);
	}
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->SetCommander(_pCommander);
	}
}

WINDMILL::E_STATE CWindmill::GetState() const
{
	return m_pWindmillBase->GetState();
}

//void CWindmill::CollectGarbageObjs()
//{
//	for (auto& pFarmland : m_vecFarmlands) {
//		pFarmland->CollectGarbageObjs();
//	}
//}
