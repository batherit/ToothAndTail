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
	// �� ����
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
		
	//GetSceneManager()->SetNextScene(new CTestScene(*this));		// �ʱ�� ����
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
	// ���а� �������ٸ�,
	if (CTAT_World::RESULT_YET != m_eGameResult) return;

	bool bIsAIAlive = false;
	bool bIsPlayerAlive = false;
	CWindmill* pWindmill = nullptr;
	for (auto& pObj : GetListObjs()) {
		pWindmill = dynamic_cast<CWindmill*>(pObj);
		if (pWindmill) {
			// �� AI ���мҰ� �ϳ��� ���������� �¸��� ���� �ƴ�.
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
		// �й�
		m_eGameResult = CTAT_World::RESULT_LOSE;
		return;
	}
}

void CTAT_World::LoadResources(void)
{
	// ���� �ε�
	CSoundMgr::GetInstance()->Initialize();

	// ��
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP"))) return;

	// Ÿ��
	// 1) �븻
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile0.png", L"TILE_NORMAL"))) return;
	// 2) ���ŷ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile1.png", L"TILE_BLOCKING"))) return;
	// 3) OK
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile5.png", L"TILE_GREEN"))) return;
	// 4) NO
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile3.png", L"TILE_RED"))) return;

	// ����
	// 1) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Stone/Stone%d.png", L"Deco", L"Stone", 3))) return;
	// 2) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Tree/Tree%d.png", L"Deco", L"Tree", 11))) return;

	// �����
	// 1) ���
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners.png", L"COM_COMMONER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners_tint.png", L"COM_COMMONER_TINT"))) return;
	// 2) �ں���
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists.png", L"COM_CAPITALIST"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists_tint.png", L"COM_CAPITALIST_TINT"))) return;
	// 3) �屳
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military.png", L"COM_MILITARY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military_tint.png", L"COM_MILITARY_TINT"))) return;
	// 4) ������
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy.png", L"COM_CLERGY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy_tint.png", L"COM_CLERGY_TINT"))) return;

	// ���м�
	// 1) ���м� ���̽�
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base.png", L"WINDMILL_BASE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base_tint.png", L"WINDMILL_BASE_TINT"))) return;
	// 2) ���м� �ͺ�(turbine)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper.png", L"WINDMILL_TURBINE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper_tint.png", L"WINDMILL_TURBINE_TINT"))) return;
	// 3) ���м� ���� ���̽�
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_bib_forest.png", L"WINDMILL_GROUND"))) return;
	// ����
	// 1) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/farmland.png", L"FARMLAND"))) return;
	// 2) ��
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_field_crop_sheet.png", L"FARMLAND_CROP"))) return;
	// ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig.png", L"PIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig_tint.png", L"PIG_TINT"))) return;

	// ����
	// 1) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1.png", L"TUNNEL_SMALL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1_tint.png", L"TUNNEL_SMALL_TINT"))) return;
	// 2) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2.png", L"TUNNEL_MIDDLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2_tint.png", L"TUNNEL_MIDDLE_TINT"))) return;
	// 3) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3.png", L"TUNNEL_BIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3_tint.png", L"TUNNEL_BIG_TINT"))) return;

	// ���� //û�ָ�(SQUIRREL), ������(LIZARD), �δ���(MOLE), ����ũ(SKUNK), ���Ҹ�(BADGER), ����(FOX)
	// 1) û�ָ� (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel.png", L"SQUIRREL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel_tint.png", L"SQUIRREL_TINT"))) return;
	// 2) ������ (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard.png", L"LIZARD"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard_tint.png", L"LIZARD_TINT"))) return;
	// 3) �δ��� (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole.png", L"MOLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole_tint.png", L"MOLE_TINT"))) return;
	// 4) ����ũ (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk.png", L"SKUNK"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk_tint.png", L"SKUNK_TINT"))) return;
	// 5) ���Ҹ� (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger.png", L"BADGER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_muzzleflash.png", L"BADGER_FLASH"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_tint.png", L"BADGER_TINT"))) return;
	// 6) ���� (���� ����)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox.png", L"FOX"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox_tint.png", L"FOX_TINT"))) return;

	// źȯ
	// ���� źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_medkit.png", L"PIG_BULLET"))) return;
	// û�ָ� źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_artillery.png", L"SQUIRREL_BULLET"))) return;
	// ������ źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/thrownspear.png", L"LIZARD_BULLET"))) return;
	// �δ����� ���� �����̴�.
	// ����ũ źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_artillery.png", L"SKUNK_BULLET"))) return;
	// ���Ҹ� źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_minigun1.png", L"BADGER_BULLET"))) return;
	// ���� źȯ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_sniper.png", L"FOX_BULLET"))) return;

	// ���� ����Ʈ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/particles.png", L"BURST"))) return;

	// UI ��Ʋ��
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/ui.png", L"UI_SET"))) return;

	// Ÿ��Ʋ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Title/title.png", L"TITLE"))) return;
	// ��ư
	// ��ư ��׶���
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttonbg1_small.png", L"BUTTON"))) return;
	// 1) ��ư ON
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_on.png", L"BUTTON_ON"))) return;
	// 2) ��ư OFF
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_off.png", L"BUTTON_OFF"))) return;

	// �̴ϸ�
	// 1) �̴ϸ�
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map1.png", L"MINIMAP"))) return;
	// 2) �̴ϸ� ǥ��
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/unit.png", L"MINIMAP_SIGN"))) return;
	
	// ����ƼŬ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Particle/particle.png", L"PARTICLE"))) return;
}
