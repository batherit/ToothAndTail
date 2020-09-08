#include "stdafx.h"
#include "CTAT_World.h"
#include "CSceneMgr.h"
#include "CTestScene.h"
#include "CTitleScene.h"
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
		
	//GetSceneManager()->SetNextScene(new CTestScene(*this));		// ÃÊ±â¾À ¼¼ÆÃ
	GetSceneManager()->SetNextScene(new CTitleScene(*this));
	GetSceneManager()->RequestSceneInit();
	m_pCamera = new CCamera(*this, nullptr, 0.f, 0.f);
	SetMainCamera(m_pCamera);		//.. ¸ÞÀÎ Ä«¸Þ¶ó ¼¼ÆÃ
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
	// ¸Ê
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP"))) return;

	// Å¸ÀÏ
	// 1) ³ë¸»
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile0.png", L"TILE_NORMAL"))) return;
	// 2) ºí·ÎÅ·
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile1.png", L"TILE_BLOCKING"))) return;
	// 3) OK
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile5.png", L"TILE_GREEN"))) return;
	// 4) NO
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile3.png", L"TILE_RED"))) return;

	// µ¥ÄÚ
	// 1) ¹ÙÀ§
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Stone/Stone%d.png", L"Deco", L"Stone", 3))) return;
	// 2) ³ª¹«
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Tree/Tree%d.png", L"Deco", L"Tree", 11))) return;

	// ±â¼öµé
	// 1) Æò¹Î
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners.png", L"COM_COMMONER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_commoners_tint.png", L"COM_COMMONER_TINT"))) return;
	// 2) ÀÚº»°¡
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists.png", L"COM_CAPITALIST"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_capitalists_tint.png", L"COM_CAPITALIST_TINT"))) return;
	// 3) Àå±³
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military.png", L"COM_MILITARY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_military_tint.png", L"COM_MILITARY_TINT"))) return;
	// 4) ¼ºÁ÷ÀÚ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy.png", L"COM_CLERGY"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/commander_clergy_tint.png", L"COM_CLERGY_TINT"))) return;

	// Á¦ºÐ¼Ò
	// 1) Á¦ºÐ¼Ò º£ÀÌ½º
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base.png", L"WINDMILL_BASE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_base_tint.png", L"WINDMILL_BASE_TINT"))) return;
	// 2) Á¦ºÐ¼Ò ÅÍºó(turbine)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper.png", L"WINDMILL_TURBINE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_topper_tint.png", L"WINDMILL_TURBINE_TINT"))) return;
	// 3) Á¦ºÐ¼Ò ÅäÁö º£ÀÌ½º
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_bib_forest.png", L"WINDMILL_GROUND"))) return;
	// ³óÁö
	// 1) ³óÁö
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/farmland.png", L"FARMLAND"))) return;
	// 2) º­
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/Windmill/windmill_field_crop_sheet.png", L"FARMLAND_CROP"))) return;
	// µÅÁö
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig.png", L"PIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/pig_tint.png", L"PIG_TINT"))) return;

	// ¶¥±¼
	// 1) ¼ÒÇü
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1.png", L"TUNNEL_SMALL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens1_tint.png", L"TUNNEL_SMALL_TINT"))) return;
	// 2) ÁßÇü
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2.png", L"TUNNEL_MIDDLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens2_tint.png", L"TUNNEL_MIDDLE_TINT"))) return;
	// 3) ´ëÇü
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3.png", L"TUNNEL_BIG"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Structure/GamePlay/structure_warrens3_tint.png", L"TUNNEL_BIG_TINT"))) return;

	// À¯´Ö //Ã»¼Ö¸ð(SQUIRREL), µµ¸¶¹ì(LIZARD), µÎ´õÁö(MOLE), ½ºÄÈÅ©(SKUNK), ¿À¼Ò¸®(BADGER), ¿©¿ì(FOX)
	// 1) Ã»¼Ö¸ð (¼ÒÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel.png", L"SQUIRREL"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/squirrel_tint.png", L"SQUIRREL_TINT"))) return;
	// 2) µµ¸¶¹ì (¼ÒÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard.png", L"LIZARD"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/lizard_tint.png", L"LIZARD_TINT"))) return;
	// 3) µÎ´õÁö (¼ÒÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole.png", L"MOLE"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/mole_tint.png", L"MOLE_TINT"))) return;
	// 4) ½ºÄÈÅ© (ÁßÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk.png", L"SKUNK"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/skunk_tint.png", L"SKUNK_TINT"))) return;
	// 5) ¿À¼Ò¸® (´ëÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger.png", L"BADGER"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_muzzleflash.png", L"BADGER_FLASH"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/badger_tint.png", L"BADGER_TINT"))) return;
	// 6) ¿©¿ì (´ëÇü ¶¥±¼)
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox.png", L"FOX"))) return;
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Actors/fox_tint.png", L"FOX_TINT"))) return;

	// ÅºÈ¯
	// µÅÁö ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_medkit.png", L"PIG_BULLET"))) return;
	// Ã»¼Ö¸ð ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/bullet_artillery.png", L"SQUIRREL_BULLET"))) return;
	// µµ¸¶¹ì ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/thrownspear.png", L"LIZARD_BULLET"))) return;
	// µÎ´õÁö´Â ±ÙÁ¢ °ø°ÝÀÌ´Ù.
	// ½ºÄÈÅ© ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_artillery.png", L"SKUNK_BULLET"))) return;
	// ¿À¼Ò¸® ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_minigun1.png", L"BADGER_BULLET"))) return;
	// ¿©¿ì ÅºÈ¯
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Bullets/tracer_sniper.png", L"FOX_BULLET"))) return;

	// Æø¹ß ÀÌÆåÆ®
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/particles.png", L"BURST"))) return;

	// UI
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Atlas/ui.png", L"UI_SET"))) return;

	// Å¸ÀÌÆ²
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Title/title.png", L"TITLE"))) return;
	// ¹öÆ°
	// ¹öÆ° ¹é±×¶ó¿îµå
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttonbg1_small.png", L"BUTTON"))) return;
	// 1) ¹öÆ° ON
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_on.png", L"BUTTON_ON"))) return;
	// 2) ¹öÆ° OFF
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/UI/Buttons/buttontab1_off.png", L"BUTTON_OFF"))) return;
}
