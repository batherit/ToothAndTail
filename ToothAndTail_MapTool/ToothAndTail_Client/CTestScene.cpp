#include "stdafx.h"
#include "CTestScene.h"
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


CTestScene::CTestScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld)
{
	ResetScene();
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::ResetScene(void)
{
	Release();

	CMapLoader* pOldMapLoder = m_rGameWorld.SetMapLoader(new CMapLoader(m_rGameWorld, L"../Data/MapData.dat"));
	SafelyDeleteObj(pOldMapLoder);

	// 1) 기수 테스트
	/*m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, -200.f, 0.f, CCommander::COM_TYPE_MILITARY, D3DCOLOR_ARGB(255, 0, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, -100.f, 0.f, CCommander::COM_TYPE_CAPITALIST, D3DCOLOR_ARGB(255, 0, 0, 255)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, 0.f, 0.f, CCommander::COM_TYPE_CLERGY, D3DCOLOR_ARGB(255, 255, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, 100.f, 0.f, CCommander::COM_TYPE_COMMONER, D3DCOLOR_ARGB(255, 255, 0, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CTurbine(m_rGameWorld, 0.f, 0.f, nullptr));*/

	// 2) 기수 및 제분소 렌더레이어 테스트
	// 플레이어 제분소
	m_pCommander[0] = new CCommander(m_rGameWorld, 0.f, 200.f, CCommander::COM_TYPE_COMMONER, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_pCommander[0]->SetPrivateCamera(new CCamera(m_rGameWorld, m_pCommander[0]));
	m_rGameWorld.SetMainCamera(m_pCommander[0]->GetPrivateCamera());
	//m_rGameWorld.GetMainCamera()->SetParent(m_pCommander[0]); // 좀 고쳐야 된다.
	
															  //m_rGameWorld.GetMainCamera()->SetXY(871.f,3395.f);
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander[0]);
	CWindmill* pWindmill = new CWindmill(m_rGameWorld, 288, m_pCommander[0]);
	D3DXVECTOR3 pNewPos = pWindmill->GetXY();
	pNewPos.y += TILE_HEIGHT * 4 * BASE_SCALE ;
	m_pCommander[0]->SetXY(pNewPos);
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
	
	//m_pCommander[1] = new CCommander(m_rGameWorld, -200.f, 0.f, CCommander::COM_TYPE_MILITARY, D3DCOLOR_ARGB(255, 0, 255, 0));
	////m_rGameWorld.GetMainCamera()->SetParent(m_pCommander[1]); // 좀 고쳐야 된다.
	//m_rGameWorld.GetListObjs().emplace_back(m_pCommander[1]);
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 323, m_pCommander[1]));
	//
	//m_pCommander[2] = new CCommander(m_rGameWorld, 0.f, 0.f, CCommander::COM_TYPE_CLERGY, D3DCOLOR_ARGB(255, 255, 255, 0));
	////m_rGameWorld.GetMainCamera()->SetParent(m_pCommander[2]); // 좀 고쳐야 된다.
	//m_rGameWorld.GetListObjs().emplace_back(m_pCommander[2]);
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 1933, m_pCommander[2]));

	//m_pCommander[3] = new CCommander(m_rGameWorld, -100.f, 0.f, CCommander::COM_TYPE_CAPITALIST, D3DCOLOR_ARGB(255, 0, 0, 255));
	////m_rGameWorld.GetMainCamera()->SetParent(m_pCommander[3]); // 좀 고쳐야 된다.
	//m_rGameWorld.GetListObjs().emplace_back(m_pCommander[3]);
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 1968, m_pCommander[3]));
	
	
	
	
	
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 0.f, 0.f, CWindmill::STATE_BUILDING, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 0.f, 0.f, m_pCommander));

	// 3) 농장 테스트
	//m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, 72.f * BASE_SCALE, 0.f, CFarmland::STATE_UNOCCUPIED, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, 0.f, 48.f * BASE_SCALE, CFarmland::STATE_BUILDING, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, -72.f * BASE_SCALE, 0.f, CFarmland::STATE_OCCUPIED, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, (-72.f + 36.f) * BASE_SCALE, 24.f* BASE_SCALE, CFarmland::STATE_DESTROYED, m_pCommander));

	// 4) 땅굴 테스트
	//m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, -300.f, 300.f, CTunnel::SIZE_SMALL, nullptr, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, 0.f, 300.f, CTunnel::SIZE_MIDDLE, nullptr, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, 300.f, 300.f, CTunnel::SIZE_BIG, nullptr, m_pCommander));
}

int CTestScene::Update(float _fDeltaTime)
{
	for (auto& pTile : m_rGameWorld.GetMapLoader()->GetTiles()) {
		pTile->Update(_fDeltaTime);
	}
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->Update(_fDeltaTime);
	}
	return 0;
}

void CTestScene::LateUpdate(void)
{
	for (int i = 0; i < 2; ++i) {
		//m_rGameWorld.GetMapLoader()->PushObjectInMap(m_pCommander[i]);
		//CTile* pTile = m_rGameWorld.GetMapLoader()->GetTile(m_pCommander[i]->GetXY());
		//pTile->RegisterObjOnTile(m_pCommander[i]);
		//for (auto& pBlockingTile : m_rGameWorld.GetMapLoader()->GetBlockingTiles()) {
		//	pBlockingTile->PushOutOfTile(m_pCommander[i]);
		//}
		
	}

	CComDepObj* pComDepObj = nullptr;
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pComDepObj = dynamic_cast<CComDepObj*>(pObj);
		if (!pComDepObj) continue;
		m_rGameWorld.GetMapLoader()->PushObjectInMap(pComDepObj);
	}
	

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->LateUpdate();
	}

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->CollectGarbageObjs();
	}

	CollectGarbageObjs(m_rGameWorld.GetListObjs());
}

void CTestScene::Release(void)
{
	CMapLoader* pOldMapLoader = m_rGameWorld.SetMapLoader(nullptr);
	SafelyDeleteObj(pOldMapLoader);
	SafelyDeleteObjs(m_rGameWorld.GetListObjs());
	SafelyDeleteObj(m_pInGameUI);
}

void CTestScene::Render(CCamera * _pCamera)
{
	m_rGameWorld.GetMapLoader()->RenderMap(_pCamera);
	m_rGameWorld.GetMapLoader()->RenderTile(_pCamera);

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
}
