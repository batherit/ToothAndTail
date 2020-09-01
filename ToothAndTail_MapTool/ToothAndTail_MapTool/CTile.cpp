#include "stdafx.h"
#include "CTile.h"
#include "CTextureMgr.h"



CTile::CTile(CGameWorld & _rGameWorld, float _fX, float _fY, CTile::E_TYPE _eTileType)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, TILE_WIDTH, TILE_HEIGHT),
	m_eTileType(_eTileType)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_NORMAL"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_BLOCKING"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_GREEN"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_RED"));

	SetAnimIndex(_eTileType);
}

CTile::~CTile()
{
}
