#pragma once
#include "CTexture.h"
class CMultiTexture final :
	public CTexture
{
public:
	explicit CMultiTexture();
	virtual ~CMultiTexture();
public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT InsertTexture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & rCount = 0) override;
	virtual const TextureInfo * GetTexture(const wstring & wstrStateKey = L"", const DWORD & rIndex = 0) override;
	virtual void ReleaseTexture() override;

private:
	// StateKey 
	// 하나의 STateKey(행동)에 여러개의 그림이 들어갈 수 있기 때문. 
	// 예를 들어 공격이란 StateKey 값에 20장의 그림을 넣고 
	//추후 인덱스를 빠르게 증가시켜 마치 애니메이션 돌아가듯 출력 할 것. 
	map<wstring, vector<TextureInfo*>> m_mapMultiTexture;
};

