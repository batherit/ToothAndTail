#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEXTURE_TYPE { TYPE_SINGLE, TYPE_MULTI, TYPE_END };

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TextureInfo* GetTextureInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwImgIndex = 0);
	HRESULT InsertTexture(const TEXTURE_TYPE& eTextureType, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwImgIndex = 0);
	void ReleaseTextureMgr();

private:
	// ObjectKey
	map<wstring, CTexture*> m_mapTexture;
};

