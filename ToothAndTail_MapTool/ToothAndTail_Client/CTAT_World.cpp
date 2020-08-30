#include "stdafx.h"
#include "CTAT_World.h"
#include "CSceneMgr.h"
#include "CTestScene.h"
#include "CTextureMgr.h"
#include "CTimer.h"
#include "CCamera.h"


CTAT_World::CTAT_World()
{
}


CTAT_World::~CTAT_World()
{
	Release();
}

LRESULT CTAT_World::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (zDelta > 0) {
			m_pCamera->ZoomIn(0.05f);
		}
		else {
			m_pCamera->ZoomOut(0.05f);
		}
		break;
	}
	}

	return 0;
}

void CTAT_World::Ready(void)
{
	LoadTextures();
		
	GetSceneManager()->SetNextScene(new CTestScene(*this));		// 초기씬 세팅
	GetSceneManager()->RequestSceneInit();
	m_pCamera = new CCamera(*this, nullptr, 0.f, 0.f);
	SetMainCamera(m_pCamera);		//.. 메인 카메라 세팅
}

void CTAT_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();

	GetSceneManager()->Update(fDeltaTime);
	GetMainCamera()->Update(fDeltaTime);
}

void CTAT_World::LateUpdate(void)
{
	GetSceneManager()->LateUpdate();
	GetMainCamera()->LateUpdate();
}

void CTAT_World::Render(void)
{
	StartRender();
	GetSceneManager()->Render(GetMainCamera());
	EndRender();
}

void CTAT_World::Release(void)
{
	CKeyMgr::DestroyInstance();
	CTextureMgr::DestroyInstance();
	SafelyDeleteObj(m_pCamera);
}

void CTAT_World::LoadTextures(void)
{
	// 맵
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP");

	// 기수들
	// 1) 평민
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners.png", L"COM_COMMONER");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners_tint.png", L"COM_COMMONER_TINT");
	// 2) 자본가
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists.png", L"COM_CAPITALIST");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists_tint.png", L"COM_CAPITALIST_TINT");
	// 3) 장교
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military.png", L"COM_MILITARY");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military_tint.png", L"COM_MILITARY_TINT");
	// 4) 성직자
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy.png", L"COM_CLERGY");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy_tint.png", L"COM_CLERGY_TINT");

	// 제분소
	// 1) 제분소 베이스
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base.png", L"WINDMILL_BASE");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base_tint.png", L"WINDMILL_BASE_TINT");
	// 2) 제분소 터빈(turbine)
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper.png", L"WINDMILL_TURBINE");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper_tint.png", L"WINDMILL_TURBINE_TINT");
	// 3) 제분소 토지 베이스
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_bib_forest.png", L"WINDMILL_GROUND");
	// 농지
	// 1) 농지
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/farmland.png", L"FARMLAND");
	// 2) 벼
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_field_crop_sheet.png", L"FARMLAND_CROP");
	// 돼지
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig.png", L"PIG");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig_tint.png", L"PIG_TINT");

	// 땅굴
	// 1) 소형
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1.png", L"TUNNEL_SMALL");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1_tint.png", L"TUNNEL_SMALL_TINT");
	// 2) 중형
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2.png", L"TUNNEL_MIDDLE");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2_tint.png", L"TUNNEL_MIDDLE_TINT");
	// 3) 대형
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3.png", L"TUNNEL_BIG");
	CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3_tint.png", L"TUNNEL_BIG_TINT");
}
