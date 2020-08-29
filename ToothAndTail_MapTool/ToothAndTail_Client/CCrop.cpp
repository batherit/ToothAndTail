#include "stdafx.h"
#include "CCrop.h"
#include "CFarmland.h"
#include "CTextureMgr.h"


CCrop::CCrop(CGameWorld & _rGameWorld, CFarmland * _pFarmland, float _fX, float _fY, int _iCropIndex)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, CROP_WIDTH, CROP_HEIGHT)
{
	SetRenderLayer(10);
	SetParent(_pFarmland);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"FARMLAND_CROP"));

	AnimInfo stAnimInfo(0, 4, _iCropIndex, 1, 0.f, 0, false);
	SetNewAnimInfo(stAnimInfo);
}

CCrop::~CCrop()
{
}
