#include "stdafx.h"
#include "CTestScene.h"
#include "CGameWorld.h"
#include "CCommander.h"
#include "CWindmill.h"
#include "CFarmland.h"
#include "CTunnel.h"
#include "CCamera.h"
#include "CTextureMgr.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CDeco.h"


CTestScene::CTestScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld)
{
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::ResetScene(void)
{
	Release();

	m_pMapLoader = new CMapLoader(m_rGameWorld, L"../Data/MapData.dat");

	// 1) 기수 테스트
	/*m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, -200.f, 0.f, CCommander::COM_TYPE_MILITARY, D3DCOLOR_ARGB(255, 0, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, -100.f, 0.f, CCommander::COM_TYPE_CAPITALIST, D3DCOLOR_ARGB(255, 0, 0, 255)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, 0.f, 0.f, CCommander::COM_TYPE_CLERGY, D3DCOLOR_ARGB(255, 255, 255, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CCommander(m_rGameWorld, 100.f, 0.f, CCommander::COM_TYPE_COMMONER, D3DCOLOR_ARGB(255, 255, 0, 0)));
	m_rGameWorld.GetListObjs().emplace_back(new CTurbine(m_rGameWorld, 0.f, 0.f, nullptr));*/

	// 2) 기수 및 제분소 렌더레이어 테스트
	m_pCommander = new CCommander(m_rGameWorld, 0.f, 200.f, CCommander::COM_TYPE_COMMONER, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_rGameWorld.GetMainCamera()->SetParent(m_pCommander);
	m_rGameWorld.GetListObjs().emplace_back(m_pCommander);
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, -300.f, 0.f, CWindmill::STATE_UNOCCUPIED, m_pCommander));
	//m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 0.f, 0.f, CWindmill::STATE_BUILDING, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CWindmill(m_rGameWorld, 0.f, 0.f, CWindmill::STATE_OCCUPIED, m_pCommander));

	// 3) 농장 테스트
	m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, 72.f * BASE_SCALE, 0.f, CFarmland::STATE_UNOCCUPIED, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, 0.f, 48.f * BASE_SCALE, CFarmland::STATE_BUILDING, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, -72.f * BASE_SCALE, 0.f, CFarmland::STATE_OCCUPIED, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CFarmland(m_rGameWorld, (-72.f + 36.f) * BASE_SCALE, 24.f* BASE_SCALE, CFarmland::STATE_DESTROYED, m_pCommander));

	// 4) 땅굴 테스트
	m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, -300.f, 300.f, CTunnel::SIZE_SMALL, nullptr, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, 0.f, 300.f, CTunnel::SIZE_MIDDLE, nullptr, m_pCommander));
	m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, 300.f, 300.f, CTunnel::SIZE_BIG, nullptr, m_pCommander));
}

int CTestScene::Update(float _fDeltaTime)
{
	for (auto& rObj : m_rGameWorld.GetListObjs()) {
		rObj->Update(_fDeltaTime);
	}
	return 0;
}

void CTestScene::LateUpdate(void)
{
	m_pMapLoader->PushObjectInMap(m_pCommander);
	for (auto& pBlockingTile : m_pMapLoader->GetBlockingTiles()) {
		pBlockingTile->PushOutOfTile(m_pCommander);
	}

	for (auto& rObj : m_rGameWorld.GetListObjs()) {
		rObj->LateUpdate();
	}

	CollectGarbageObjs(m_rGameWorld.GetListObjs());
}

void CTestScene::Release(void)
{
	SafelyDeleteObj(m_pMapLoader);
	SafelyDeleteObjs(m_rGameWorld.GetListObjs());
	m_pCommander = nullptr;
}

void CTestScene::Render(CCamera * _pCamera)
{
	if(m_pMapLoader) m_pMapLoader->RenderMap(_pCamera);
	if(m_pMapLoader) m_pMapLoader->RenderTile(_pCamera);

	m_rGameWorld.RenderListObjs(_pCamera, [](CObj* pObj1, CObj* pObj2) {
		if (pObj1->GetRenderLayer() < pObj2->GetRenderLayer()) return true;
		else if (pObj1->GetRenderLayer() > pObj2->GetRenderLayer()) return false;

		return pObj1->GetBottom() < pObj2->GetBottom();
	});
}
