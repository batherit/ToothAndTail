#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const DWORD& rCount = 0) PURE;
	virtual const TextureInfo* GetTexture(const wstring& wstrStateKey = L"",
		const DWORD& rIndex = 0) PURE;
public:
	virtual void ReleaseTexture() PURE;
};

