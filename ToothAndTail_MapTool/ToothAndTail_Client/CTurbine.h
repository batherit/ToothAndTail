#pragma once
#include "CComDepObj.h"

class CCommander;
class CCamera;
class CTurbine :
	public CComDepObj
{
public:
	enum E_STATE { STATE_BUILDING, STATE_COMPLETED, STATE_END };

public:
	CTurbine(CGameWorld& _rGameWorld, float _fX, float _fY, CTurbine::E_STATE _eState = CTurbine::STATE_BUILDING, CCommander* _pCommander = nullptr);
	virtual ~CTurbine();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

private:
	CTurbine::E_STATE m_eState = CTurbine::STATE_BUILDING;
};

