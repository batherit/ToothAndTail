#pragma once
#include "CComDepObj.h"

class CCommander;
class CTurbine;
class CWindmillBase :
	public CComDepObj
{
public:
	CWindmillBase(CGameWorld& _rGameWorld, float _fX, float _fY, WINDMILL::E_STATE _eState, CCommander* _pCommander = nullptr);
	virtual ~CWindmillBase();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void) override;
	virtual float GetFloor() const override { return GetBottom() - 35.f; }
	void SetWindmillBaseState(WINDMILL::E_STATE _eState);
	WINDMILL::E_STATE GetState() const { return m_eState; }

private:
	WINDMILL::E_STATE m_eState = WINDMILL::STATE_BUILDING;
	CTurbine* m_pTurbine = nullptr;
};

