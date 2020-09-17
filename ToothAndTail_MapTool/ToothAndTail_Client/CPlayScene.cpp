#include "stdafx.h"
#include "CPlayScene.h"
#include "CGameWorld.h"
#include "CCommander.h"
#include "CCommanderAI.h"
#include "CWindmill.h"
#include "CFarmland.h"
#include "CTunnel.h"
#include "CCamera.h"
#include "CTextureMgr.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CDeco.h"
#include "CUI_InGameUI.h"
#include "CUI_FadeInOut.h"
#include "CObserver.h"



CPlayScene::CPlayScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld)
{
	ResetScene();
	CSoundMgr::GetInstance()->PlayBGM(TEXT("Background.wav"));
	ShowCursor(false);
}

CPlayScene::~CPlayScene()
{
	Release();
	ShowCursor(true);
}

void CPlayScene::ResetScene(void)
{
	Release();

	// 맵 생성
	CMapLoader* pOldMapLoder = m_rGameWorld.SetMapLoader(new CMapLoader(m_rGameWorld, L"../Data/MapData.dat"));
	SafelyDeleteObj(pOldMapLoder);

	// 옵저버 생성
	m_pObserver = new CObserver(m_rGameWorld);

	// 2) 기수 및 제분소 렌더레이어 테스트
	// 플레이어 제분소
	m_pCommander[0] = new CCommander(m_rGameWorld, 0.f, 200.f, CCommander::COM_TYPE_COMMONER, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_rGameWorld.SetMainCamera(m_pCommander[0]->GetPrivateCamera());
	//m_rGameWorld.GetMainCamera()->SetParent(m_pCommander[0]); // 좀 고쳐야 된다.
	
															  //m_rGameWorld.GetMainCamera()->SetXY(871.f,3395.f);
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander[0]);
	CWindmill* pWindmill = new CWindmill(m_rGameWorld, 288, m_pCommander[0]);
	D3DXVECTOR3 pNewPos = pWindmill->GetXY();
	pNewPos.y += TILE_HEIGHT * 4 * BASE_SCALE ;
	m_pCommander[0]->SetXY(pNewPos);

	// m_pCommander[0]를 게임월드의 플레이어로 설정함.
	m_rGameWorld.SetPlayer(m_pCommander[0]);
	m_rGameWorld.GetListObjs().emplace_back(pWindmill);

	// 인게임 UI 생성
	m_pInGameUI = new CUI_InGameUI(m_rGameWorld, m_pCommander[0]);


	m_pCommander[1] = new CCommanderAI(m_rGameWorld, -200.f, 0.f, CCommander::COM_TYPE_MILITARY, D3DCOLOR_ARGB(255, 0, 255, 0));
	pNewPos.x += TILE_WIDTH * BASE_SCALE;
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander[1]);
	pWindmill = new CWindmill(m_rGameWorld, 323, m_pCommander[1]);
	pNewPos = pWindmill->GetXY();
	pNewPos.y += TILE_HEIGHT * 4 * BASE_SCALE;
	m_pCommander[1]->SetXY(pNewPos);
	m_rGameWorld.GetListObjs().emplace_back(pWindmill);

	// 점령되지 않은 제분소
	pWindmill = new CWindmill(m_rGameWorld, 813, nullptr);
	m_rGameWorld.GetListObjs().emplace_back(pWindmill);


	m_pCommander[2] = new CCommanderAI(m_rGameWorld, 0.f, 0.f, CCommander::COM_TYPE_CLERGY, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander[2]);
	pWindmill = new CWindmill(m_rGameWorld, 1933, m_pCommander[2]);
	pNewPos = pWindmill->GetXY();
	pNewPos.y += TILE_HEIGHT * 4 * BASE_SCALE;
	m_pCommander[2]->SetXY(pNewPos);
	m_rGameWorld.GetListObjs().emplace_back(pWindmill);

	m_pCommander[3] = new CCommanderAI(m_rGameWorld, -100.f, 0.f, CCommander::COM_TYPE_CAPITALIST, D3DCOLOR_ARGB(255, 0, 0, 255));
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander[3]);
	pWindmill = new CWindmill(m_rGameWorld, 1968, m_pCommander[3]);
	pNewPos = pWindmill->GetXY();
	pNewPos.y += TILE_HEIGHT * 4 * BASE_SCALE;
	m_pCommander[3]->SetXY(pNewPos);
	m_rGameWorld.GetListObjs().emplace_back(pWindmill);
	
	// 페이드인/아웃 효과
	m_pFadeInOutUI = new CUI_FadeInOut(m_rGameWorld);
	m_pFadeInOutUI->StartFadeInOut(1.2f, true);
}

int CPlayScene::Update(float _fDeltaTime)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_TAB)) {
		ToggleBoolean(m_bIsTilesRenderring);
	}

	if (m_bIsObserverMode) {
		m_pObserver->Update(_fDeltaTime);
	}

	for (auto& pTile : m_rGameWorld.GetMapLoader()->GetTiles()) {
		pTile->Update(_fDeltaTime);
	}
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->Update(_fDeltaTime);
	}

	m_pFadeInOutUI->Update(_fDeltaTime);
	return 0;
}

void CPlayScene::LateUpdate(void)
{
	CComDepObj* pComDepObj = nullptr;
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pComDepObj = dynamic_cast<CComDepObj*>(pObj);
		if (!pComDepObj) continue;
		m_rGameWorld.GetMapLoader()->PushObjectInMap(pComDepObj);
	}
	
	m_rGameWorld.GetMapLoader()->PushObjectInMap(m_pObserver);

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->LateUpdate();
	}

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->CollectGarbageObjs();
	}

	CollectGarbageObjs(m_rGameWorld.GetListObjs());
}

void CPlayScene::Release(void)
{
	CMapLoader* pOldMapLoader = m_rGameWorld.SetMapLoader(nullptr);
	SafelyDeleteObj(pOldMapLoader);
	SafelyDeleteObj(m_pObserver);
	SafelyDeleteObjs(m_rGameWorld.GetListObjs());
	SafelyDeleteObj(m_pInGameUI);
	SafelyDeleteObj(m_pFadeInOutUI);
	m_rGameWorld.SetPlayer(nullptr);
	m_rGameWorld.SetMainCamera(nullptr);
}

void CPlayScene::Render(CCamera * _pCamera)
{
	m_rGameWorld.GetMapLoader()->RenderMap(_pCamera);
	if(m_bIsTilesRenderring) m_rGameWorld.GetMapLoader()->RenderTile(_pCamera);

	auto& rRenderList = m_rGameWorld.GetRenderList();
	rRenderList.clear();
	for (auto& pDeco : m_rGameWorld.GetMapLoader()->GetDecos()) {
		rRenderList.emplace_back(pDeco);
	}

	m_rGameWorld.RenderListObjs(_pCamera, false, [](CObj* pObj1, CObj* pObj2) {
		if (pObj1->GetRenderLayer() < pObj2->GetRenderLayer()) return true;
		else if (pObj1->GetRenderLayer() > pObj2->GetRenderLayer()) return false;

		return pObj1->GetFloor() < pObj2->GetFloor();
	});

	m_pInGameUI->Render(_pCamera);
	m_pFadeInOutUI->Render(nullptr);
}

LRESULT CPlayScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{	
		case VK_NUMPAD0:
			m_bIsObserverMode = false;
			m_rGameWorld.SetMainCamera(m_pCommander[0]->GetPrivateCamera());
			m_pInGameUI->ChangeCommander(m_pCommander[0]);
			break;
		case VK_NUMPAD1:
			m_bIsObserverMode = false;
			m_rGameWorld.SetMainCamera(m_pCommander[1]->GetPrivateCamera());
			m_pInGameUI->ChangeCommander(m_pCommander[1]);
			break;
		case VK_NUMPAD2:
			m_bIsObserverMode = false;
			m_rGameWorld.SetMainCamera(m_pCommander[2]->GetPrivateCamera());
			m_pInGameUI->ChangeCommander(m_pCommander[2]);
			break;
		case VK_NUMPAD3:
			m_bIsObserverMode = false;
			m_rGameWorld.SetMainCamera(m_pCommander[3]->GetPrivateCamera());
			m_pInGameUI->ChangeCommander(m_pCommander[3]);
			break;
		case VK_NUMPAD4:
			if (!m_pObserver) break;
			m_bIsObserverMode = true;
			m_rGameWorld.SetMainCamera(m_pObserver->GetObserverCamera());
			m_pObserver->SetZoomMultiple(OBSERVER_MIN_ZOOM_MULTIPLE);
			break;
		case VK_NUMPAD5:
			if (!m_pObserver || !m_bIsObserverMode) break;
			m_pObserver->SetZoomMultiple(OBSERVER_MIN_ZOOM_MULTIPLE);
			m_pObserver->SetXY((MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE);
		default:
			break;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0) {
			if (m_bIsObserverMode && m_pObserver) {
				if (m_pObserver->GetZoomMultiple() > OBSERVER_MIN_ZOOM_MULTIPLE) {
					m_pObserver->ZoomIn(0.05f);
					if (m_pObserver->GetZoomMultiple() <= OBSERVER_MIN_ZOOM_MULTIPLE) {
						m_pObserver->SetZoomMultiple(OBSERVER_MIN_ZOOM_MULTIPLE);
					}
				}
			}
		}
		else {
			if (m_bIsObserverMode && m_pObserver) {
				if (m_pObserver->GetZoomMultiple() < OBSERVER_MAX_ZOOM_MULTIPLE) {
					m_pObserver->ZoomOut(0.05f);
					if (m_pObserver->GetZoomMultiple() >= OBSERVER_MAX_ZOOM_MULTIPLE) {
						m_pObserver->SetZoomMultiple(OBSERVER_MAX_ZOOM_MULTIPLE);
					}
				}
			}
		}
		break;
	}
	}

	return 0;
}
