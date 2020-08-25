#include "stdafx.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CSingleTexture.h"
#include "CMultiTexture.h"

INITIALIZE_SINGLETON(CTextureMgr)
CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	ReleaseTextureMgr();
}

const TextureInfo * CTextureMgr::GetTextureInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwImgIndex)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexture(wstrStateKey, dwImgIndex);
}

HRESULT CTextureMgr::InsertTexture(const TEXTURE_TYPE & eTextureType, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwImgIndex)
{
	map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(wstrObjectKey);

	if (iter_find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;
		switch (eTextureType)
		{
		case CTextureMgr::TYPE_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case CTextureMgr::TYPE_MULTI:
			pTexture = new CMultiTexture;
			break;
		default:
			break;
		}
		if (FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, dwImgIndex)))
		{
			ERR_MSG(L"Failed to create texture.");
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (CTextureMgr::TYPE_MULTI == eTextureType)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->InsertTexture(wstrFilePath, wstrStateKey, dwImgIndex)))
		{
			ERR_MSG(L"Failed to create multi texture.");
			return E_FAIL;
		}
	}
	return S_OK;
}

void CTextureMgr::ReleaseTextureMgr()
{
	for (auto& rPair : m_mapTexture)
		SafelyDeleteObj(rPair.second);
	m_mapTexture.clear();
}
