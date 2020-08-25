#pragma once
#include "CTexture.h"
class CSingleTexture final :
	public CTexture
{
public:
	explicit CSingleTexture();
	virtual ~CSingleTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT InsertTexture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & rCount = 0) override;
	virtual const TextureInfo * GetTexture(const wstring & wstrStateKey = L"", const DWORD & rIndex = 0) override;
	virtual void ReleaseTexture();

private:
	TextureInfo* m_pTextureInfo = nullptr;
};

