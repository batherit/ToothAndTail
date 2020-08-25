#pragma once
#include "CTexture.h"
class CMultiTexture final :
	public CTexture
{
public:
	explicit CMultiTexture();
	virtual ~CMultiTexture();
public:
	// CTexture��(��) ���� ��ӵ�
	virtual HRESULT InsertTexture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & rCount = 0) override;
	virtual const TextureInfo * GetTexture(const wstring & wstrStateKey = L"", const DWORD & rIndex = 0) override;
	virtual void ReleaseTexture() override;

private:
	// StateKey 
	// �ϳ��� STateKey(�ൿ)�� �������� �׸��� �� �� �ֱ� ����. 
	// ���� ��� �����̶� StateKey ���� 20���� �׸��� �ְ� 
	//���� �ε����� ������ �������� ��ġ �ִϸ��̼� ���ư��� ��� �� ��. 
	map<wstring, vector<TextureInfo*>> m_mapMultiTexture;
};

