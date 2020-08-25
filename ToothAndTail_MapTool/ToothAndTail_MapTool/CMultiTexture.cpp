#include "stdafx.h"
#include "CMultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	ReleaseTexture();
}

HRESULT CMultiTexture::InsertTexture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & rCount)
{
	auto& iter_find = m_mapMultiTexture.find(wstrStateKey);
	TCHAR szFilePath[MAX_PATH] = L"";
	if (m_mapMultiTexture.end() == iter_find)
	{
		for (DWORD i = 0; i < rCount; ++i)
		{
			wsprintf(szFilePath, wstrFilePath.c_str(), i);
			TextureInfo* pTextureInfo = new TextureInfo;
			ZeroMemory(pTextureInfo, sizeof(pTextureInfo));

			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTextureInfo->tImageInfo)))
			{
				ERR_MSG(L"Failed to get image info from file.");
				SafelyDeleteObj(pTextureInfo);	// 읽어들이지 못한 텍스쳐 인포는 제거한다.
				ReleaseTexture();				// 멀티 텍스처 정보가 완성되지 못했으므로 제거한다.
				return E_FAIL;
			}

			if (FAILED(D3DXCreateTextureFromFileEx(
				CGraphicDevice::GetInstance()->GetDevice(),
				szFilePath,
				pTextureInfo->tImageInfo.Width,
				pTextureInfo->tImageInfo.Height,
				pTextureInfo->tImageInfo.MipLevels,
				0,
				pTextureInfo->tImageInfo.Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				&pTextureInfo->pTexture)))
			{
				wstring wstrErrMessage = wstrFilePath + L"Create Texture Failed";
				ERR_MSG(wstrErrMessage.c_str());
				SafelyDeleteObj(pTextureInfo);	// 읽어들이지 못한 텍스쳐 인포는 제거한다.
				ReleaseTexture();				// 멀티 텍스처 정보가 완성되지 못했으므로 제거한다.
				return E_FAIL;
			}
			m_mapMultiTexture[wstrStateKey].emplace_back(pTextureInfo);
		}
	}
	return S_OK;
}

const TextureInfo * CMultiTexture::GetTexture(const wstring & wstrStateKey, const DWORD & rIndex)
{
	auto& iter_find = m_mapMultiTexture.find(wstrStateKey);
	if (iter_find == m_mapMultiTexture.end())
		return nullptr;

	//return m_mapMultiTex[wstrStateKey][rIndex];
	return iter_find->second[rIndex];
}

void CMultiTexture::ReleaseTexture()
{
	for (auto& rPair : m_mapMultiTexture)
	{
		for (auto& pTexInfo : rPair.second)
		{
			SafelyDeleteObj(pTexInfo);
		}
		rPair.second.clear();
		rPair.second.shrink_to_fit();
		//rPair.second.swap(vector<TextureInfo*>()); // 택 1 
		
	}
}
