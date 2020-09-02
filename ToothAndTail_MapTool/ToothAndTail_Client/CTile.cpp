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

CTile::CTile(CGameWorld & _rGameWorld)
	:
	CSpriteObj(_rGameWorld)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_NORMAL"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_BLOCKING"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_GREEN"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_RED"));

	SetAnimIndex(0);
}

CTile::~CTile()
{
}

void CTile::SaveInfo(HANDLE & _hfOut)
{
	//DWORD dwByte = 0;
	WriteFile(_hfOut, &m_eTileType, sizeof(m_eTileType), nullptr, nullptr);
	WriteFile(_hfOut, &m_vPos, sizeof(m_vPos), nullptr, nullptr);
	WriteFile(_hfOut, &m_vScale, sizeof(m_vScale), nullptr, nullptr);
	POINT ptSize{ m_iWidth, m_iHeight };
	WriteFile(_hfOut, &ptSize, sizeof(ptSize), nullptr, nullptr);
}

void CTile::LoadInfo(HANDLE & _hfIn)
{
	ReadFile(_hfIn, &m_eTileType, sizeof(m_eTileType), nullptr, nullptr);
	ReadFile(_hfIn, &m_vPos, sizeof(m_vPos), nullptr, nullptr);
	ReadFile(_hfIn, &m_vScale, sizeof(m_vScale), nullptr, nullptr);
	POINT ptSize;
	ReadFile(_hfIn, &ptSize, sizeof(ptSize), nullptr, nullptr);

	SetAnimIndex(m_eTileType);
	m_iWidth = ptSize.x;
	m_iHeight = ptSize.y;
}
