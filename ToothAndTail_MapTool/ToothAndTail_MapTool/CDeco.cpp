#include "stdafx.h"
#include "CDeco.h"
#include "CTextureMgr.h"

CDeco::CDeco(CGameWorld & _rGameWorld, float _fX, float _fY, CDeco::E_TYPE _eDecoType, int _iDecoID)
	:
	CSpriteObj(_rGameWorld, _fX, _fY),
	m_eDecoType(_eDecoType),
	m_iDecoID(_iDecoID)
{
	wstring wstrDecoKey = L"";
	switch (m_eDecoType)
	{
	case CDeco::TYPE_STONE:
		wstrDecoKey = L"Stone";
		break;
	case CDeco::TYPE_TREE:
		wstrDecoKey = L"Tree";
		break;
	}
	const TextureInfo* pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"Deco", wstrDecoKey, _iDecoID);
	SetSize(pTextureInfo->tImageInfo.Width, pTextureInfo->tImageInfo.Height);
	PushTexture(pTextureInfo);

	SetY(GetY() - (GetHeight() * 0.5f) + (TILE_HEIGHT >> 1));
}

CDeco::CDeco(CGameWorld & _rGameWorld, float _fX, float _fY, const wstring & _wstrDecoKey, int _iDecoID)
	:
	CSpriteObj(_rGameWorld, _fX, _fY),
	m_iDecoID(_iDecoID)
{
	if (_wstrDecoKey == L"Stone") m_eDecoType = CDeco::TYPE_STONE;
	else if (_wstrDecoKey == L"Tree") m_eDecoType = CDeco::TYPE_TREE;

	const TextureInfo* pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"Deco", _wstrDecoKey, _iDecoID);
	SetSize(pTextureInfo->tImageInfo.Width, pTextureInfo->tImageInfo.Height);
	PushTexture(pTextureInfo);

	SetY(GetY() - (GetHeight() * 0.5f) + (TILE_HEIGHT >> 1));
}

CDeco::~CDeco()
{
}
