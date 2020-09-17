#include "stdafx.h"
#include "CTAT_World.h"
#include "CSceneMgr.h"
#include "CTitleScene.h"
#include "CTextureMgr.h"
#include "CTimer.h"
#include "CCamera.h"
#include "CWindmill.h"
#include "CCommanderAI.h"


CTAT_World::CTAT_World()
{
}


CTAT_World::~CTAT_World()
{
	Release();
}

LRESULT CTAT_World::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	// 줌 고정
	/*switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (zDelta > 0) {
			if(GetMainCamera()) GetMainCamera()->ZoomIn(0.05f);
		}
		else {
			if (GetMainCamera()) GetMainCamera()->ZoomOut(0.05f);
		}
		break;
	}
	}*/

	GetSceneManager()->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);

	return 0;
}

void CTAT_World::Ready(void)
{
	LoadResources();
		
	//GetSceneManager()->SetNextScene(new CTestScene(*this));		// 초기씬 세팅
	GetSceneManager()->SetNextScene(new CTitleScene(*this));
	GetSceneManager()->RequestSceneInit();
}

void CTAT_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();

	if (m_bIsCameraEventOccurring) {
		m_pAnotherCamera->Update(fDeltaTime);
		if ((m_fCameraKeepTime -= fDeltaTime) < 0.f) {
			m_bIsCameraEventOccurring = false;
		}
		GetSceneManager()->Update(fDeltaTime * 0.4f);
	}
	else {
		GetSceneManager()->Update(fDeltaTime);
		if(GetMainCamera()) GetMainCamera()->Update(fDeltaTime);
	}
	
}

void CTAT_World::LateUpdate(void)
{
	GetSceneManager()->LateUpdate();
	if (GetMainCamera()) GetMainCamera()->LateUpdate();
}

void CTAT_World::Render(void)
{
	StartRender();
	if (m_bIsCameraEventOccurring)
		GetSceneManager()->Render(m_pAnotherCamera);
	else
		GetSceneManager()->Render(GetMainCamera());

	if (m_eGameResult == CTAT_World::RESULT_WIN) {
		CGraphicDevice::GetInstance()->RenderText(L"Victory!", D3DXVECTOR3((WINCX >> 2) + 50.f , (WINCY >> 1) + 120.f, 0.f), 5.f, D3DCOLOR_ARGB(220, 0, 0, 200));
	}
	else if(m_eGameResult == CTAT_World::RESULT_LOSE) {
		CGraphicDevice::GetInstance()->RenderText(L"Defeat..", D3DXVECTOR3((WINCX >> 2) + 50.f, (WINCY >> 1) + 120.f, 0.f), 5.f, D3DCOLOR_ARGB(220, 200, 0, 0));
	}

	EndRender();
}

void CTAT_World::Release(void)
{
	CKeyMgr::DestroyInstance();
	CTextureMgr::DestroyInstance();
	CSoundMgr::DestroyInstance();
}

void CTAT_World::SetAnotherCameraTemporarily(CCamera * _pAnotherCamera, float _fKeepTime)
{
	if (!_pAnotherCamera) return;

	m_bIsCameraEventOccurring = true;
	m_pAnotherCamera = _pAnotherCamera;
	m_fCameraKeepTime = _fKeepTime;
}

void CTAT_World::JudgetResult()
{
	// 승패가 결정났다면,
	if (CTAT_World::RESULT_YET != m_eGameResult) return;

	bool bIsAIAlive = false;
	bool bIsPlayerAlive = false;
	CWindmill* pWindmill = nullptr;
	for (auto& pObj : GetListObjs()) {
		pWindmill = dynamic_cast<CWindmill*>(pObj);
		if (pWindmill) {
			// 적 AI 제분소가 하나라도 남아있으면 승리한 것이 아님.
			if (dynamic_cast<CCommanderAI*>(pWindmill->GetCommander())) {
				bIsAIAlive = true;
			}
			if (pWindmill->GetCommander() == GetPlayer()) {
				bIsPlayerAlive = true;
			}
		}
	}
	
	if (!bIsAIAlive) {
		m_eGameResult = CTAT_World::RESULT_WIN;
		CSoundMgr::GetInstance()->PlaySound(L"Victory.wav", CSoundMgr::UI);

		return;
	}
		

	if (bIsPlayerAlive == false) {
		// 패배
		m_eGameResult = CTAT_World::RESULT_LOSE;
		return;
	}
}

void CTAT_World::LoadResources(void)
{
	// 사운드 로드
	CSoundMgr::GetInstance()->Initialize();

	// 맵
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP"))) return;

	// 타일
	// 1) 노말
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile0.png", L"TILE_NORMAL"))) return;
	// 2) 블로킹
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile1.png", L"TILE_BLOCKING"))) return;
	// 3) OK
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile5.png", L"TILE_GREEN"))) return;
	// 4) NO
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile3.png", L"TILE_RED"))) return;

	// 데코
	// 1) 바위
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Stone/Stone%d.png", L"Deco", L"Stone", 3))) return;
	// 2) 나무
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Tree/Tree%d.png", L"Deco", L"Tree", 11))) return;

	// 기수들
	// 1) 평민
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners.png", L"COM_COMMONER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners_tint.png", L"COM_COMMONER_TINT"))) return;
	// 2) 자본가
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists.png", L"COM_CAPITALIST"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists_tint.png", L"COM_CAPITALIST_TINT"))) return;
	// 3) 장교
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military.png", L"COM_MILITARY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military_tint.png", L"COM_MILITARY_TINT"))) return;
	// 4) 성직자
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy.png", L"COM_CLERGY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy_tint.png", L"COM_CLERGY_TINT"))) return;

	// 제분소
	// 1) 제분소 베이스
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base.png", L"WINDMILL_BASE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base_tint.png", L"WINDMILL_BASE_TINT"))) return;
	// 2) 제분소 터빈(turbine)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper.png", L"WINDMILL_TURBINE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper_tint.png", L"WINDMILL_TURBINE_TINT"))) return;
	// 3) 제분소 토지 베이스
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_bib_forest.png", L"WINDMILL_GROUND"))) return;
	// 농지
	// 1) 농지
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/farmland.png", L"FARMLAND"))) return;
	// 2) 벼
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_field_crop_sheet.png", L"FARMLAND_CROP"))) return;
	// 돼지
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig.png", L"PIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig_tint.png", L"PIG_TINT"))) return;

	// 땅굴
	// 1) 소형
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1.png", L"TUNNEL_SMALL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1_tint.png", L"TUNNEL_SMALL_TINT"))) return;
	// 2) 중형
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2.png", L"TUNNEL_MIDDLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2_tint.png", L"TUNNEL_MIDDLE_TINT"))) return;
	// 3) 대형
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3.png", L"TUNNEL_BIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3_tint.png", L"TUNNEL_BIG_TINT"))) return;

	// 유닛 //청솔모(SQUIRREL), 도마뱀(LIZARD), 두더지(MOLE), 스컹크(SKUNK), 오소리(BADGER), 여우(FOX)
	// 1) 청솔모 (소형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel.png", L"SQUIRREL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel_tint.png", L"SQUIRREL_TINT"))) return;
	// 2) 도마뱀 (소형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard.png", L"LIZARD"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard_tint.png", L"LIZARD_TINT"))) return;
	// 3) 두더지 (소형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole.png", L"MOLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole_tint.png", L"MOLE_TINT"))) return;
	// 4) 스컹크 (중형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk.png", L"SKUNK"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk_tint.png", L"SKUNK_TINT"))) return;
	// 5) 오소리 (대형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger.png", L"BADGER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_muzzleflash.png", L"BADGER_FLASH"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_tint.png", L"BADGER_TINT"))) return;
	// 6) 여우 (대형 땅굴)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox.png", L"FOX"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox_tint.png", L"FOX_TINT"))) return;

	// 탄환
	// 돼지 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_medkit.png", L"PIG_BULLET"))) return;
	// 청솔모 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_artillery.png", L"SQUIRREL_BULLET"))) return;
	// 도마뱀 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/thrownspear.png", L"LIZARD_BULLET"))) return;
	// 두더지는 근접 공격이다.
	// 스컹크 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_artillery.png", L"SKUNK_BULLET"))) return;
	// 오소리 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_minigun1.png", L"BADGER_BULLET"))) return;
	// 여우 탄환
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_sniper.png", L"FOX_BULLET"))) return;

	// 폭발 이펙트
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/particles.png", L"BURST"))) return;

	// UI 아틀라스
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/ui.png", L"UI_SET"))) return;

	// 타이틀
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Title/title.png", L"TITLE"))) return;
	// 버튼
	// 버튼 백그라운드
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttonbg1_small.png", L"BUTTON"))) return;
	// 1) 버튼 ON
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_on.png", L"BUTTON_ON"))) return;
	// 2) 버튼 OFF
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_off.png", L"BUTTON_OFF"))) return;

	// 미니맵
	// 1) 미니맵
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map1.png", L"MINIMAP"))) return;
	// 2) 미니맵 표식
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/unit.png", L"MINIMAP_SIGN"))) return;
	
	// 피파티클
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Particle/particle.png", L"PARTICLE"))) return;
}
