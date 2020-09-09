#pragma once
#include "CObj.h"

class CUI_Image;
class CUI_UnitSign;
class CCommander;
class CUI_BuildGauge :
	public CObj
{
public:
	CUI_BuildGauge(CGameWorld& _rGameWorld, CComDepObj* _pOwner, UNIT::E_TYPE _eUnitType, const D3DXVECTOR3& vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	virtual ~CUI_BuildGauge();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

public:
	// 외부에서 직접 갱신해주어야 함.
	void UpdateGauge(float _fCurrentVal, float _fMaxVal);
	void UpdateGauge(float _fProgress);
	void UpdateRenderColor();

private:
	CComDepObj* m_pOwner = nullptr;
	CUI_UnitSign* m_pUnitSign = nullptr;
	CUI_Image* m_pBuildGaugeFill = nullptr;

	float m_fProgress = 0.f;
	const int m_ciGaugeLength = 40;
};

