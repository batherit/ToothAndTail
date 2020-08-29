#pragma once
#include "CComDepObj.h"

class CCommander;
class CTurbine;
class CWindmillBase :
	public CComDepObj
{
public:
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };

public:
	CWindmillBase(CGameWorld& _rGameWorld, float _fX, float _fY, CWindmillBase::E_STATE _eState, CCommander* _pCommander = nullptr);
	virtual ~CWindmillBase();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void) override;

private:
	CWindmillBase::E_STATE m_eState = CWindmillBase::STATE_BUILDING;
	CTurbine* m_pTurbine = nullptr;
};

