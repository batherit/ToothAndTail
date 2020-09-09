#pragma once
#include "CObj.h"

class CUI_Image;
class CCamera;
class CCommander;
class CUI_UnitSign :
	public CObj
{
public:
	CUI_UnitSign(CGameWorld& _rGameWorld, CComDepObj* _pOwner, UNIT::E_TYPE _eUnitType, const D3DXVECTOR3& vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	virtual ~CUI_UnitSign();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

public:
	void UpdateRenderColor();
private:
	CComDepObj* m_pOwner = nullptr;
	CUI_Image* m_pUnitSignBack = nullptr;
	CUI_Image* m_pUnitSign = nullptr;
	CUI_Image* m_pUnitSignTint = nullptr;
};

