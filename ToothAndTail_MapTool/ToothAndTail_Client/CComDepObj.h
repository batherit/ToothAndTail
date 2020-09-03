#pragma once
#include "CSpriteObj.h"

class CCommander;
class CComDepObj :
	public CSpriteObj
{
public:
	CComDepObj(CGameWorld& _rGameWorld, CCommander* _pCommander, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f);
	virtual ~CComDepObj();

public:
	virtual void Render(CCamera* _pCamera) override;
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) override;
	virtual int UpdateAnim(float _fDeltaTime);
	virtual void Release(void) override;

public:
	void GenerateIdentificationTintObj(size_t _iWidth, size_t _iHeight, const wstring& _wstrTintKey, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DCOLOR GetIdentificationTint(void) {
		D3DCOLOR clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255);
		if (m_pIdentificationTintSprite)
			clIdentificationTint = m_pIdentificationTintSprite->GetRenderColor();
		return clIdentificationTint;
	}
	virtual void SetCommander(CCommander* _pCommander, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	CCommander* GetCommander(void) const { return m_pCommander; }

private:
	CCommander* m_pCommander = nullptr;
	CSpriteObj* m_pIdentificationTintSprite = nullptr; // �� ���� ���� ��ü
	int iCoveredRow = 1;
	int iCoveredCol = 1;
	POINT iCoveredPivot = { 0, 0 };
};

