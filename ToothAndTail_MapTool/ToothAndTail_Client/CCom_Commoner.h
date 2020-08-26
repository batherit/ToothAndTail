#pragma once
#include "CSpriteObj.h"
class CCom_Commoner :
	public CSpriteObj
{
public:
	CCom_Commoner(CGameWorld& _rGameWorld, float _fX, float _fY, const wstring& _strComName = L"COM_COMMONER", D3DCOLOR _clIdentificationTint_ARGB = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual ~CCom_Commoner();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

private:
	CSpriteObj* m_pIdentificationTintSprite = nullptr;
};

