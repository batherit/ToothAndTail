#include "stdafx.h"
#include "CSingleTexture.h"


CSingleTexture::CSingleTexture()
{
}


CSingleTexture::~CSingleTexture()
{
	ReleaseTexture();
}

HRESULT CSingleTexture::InsertTexture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & rCount)
{
	if(!m_pTextureInfo) m_pTextureInfo = new TextureInfo;
	ZeroMemory(m_pTextureInfo, sizeof(TextureInfo));

	// ���Ϸκ��� �̹��� ������ ���� �´�.
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTextureInfo->tImageInfo)))
	{
		ERR_MSG(L"Failed to get image info from file.");
		SafelyDeleteObj(m_pTextureInfo);
		return E_FAIL;
	}

	// ���� �̹��� ������ ������� �ؽ�ó�� �����.
	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphicDevice::GetInstance()->GetDevice(),
		wstrFilePath.c_str(),
		m_pTextureInfo->tImageInfo.Width,
		m_pTextureInfo->tImageInfo.Height,
		m_pTextureInfo->tImageInfo.MipLevels,
		0,
		m_pTextureInfo->tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_pTextureInfo->pTexture)))
	{
		wstring wstrErrMessage = wstrFilePath + L" Failed to create texture.";
		ERR_MSG(wstrErrMessage.c_str());
		SafelyDeleteObj(m_pTextureInfo);
		return E_FAIL;
	}
	return S_OK;
}

const TextureInfo * CSingleTexture::GetTexture(const wstring & wstrStateKey, const DWORD & rIndex)
{
	return m_pTextureInfo;
}

void CSingleTexture::ReleaseTexture()
{
	SafelyDeleteObj(m_pTextureInfo);
}
