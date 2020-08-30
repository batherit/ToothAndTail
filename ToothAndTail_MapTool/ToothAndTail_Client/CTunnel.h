#pragma once
#include "CComDepObj.h"

class CUnitGenerator;
class CCommander;
class CTunnel final:
	public CComDepObj 
{
public:
	enum E_SIZE { SIZE_SMALL, SIZE_MIDDLE, SIZE_BIG, SIZE_END };

public:
	CTunnel(CGameWorld& _rGameWorld, float _fX, float _fY, CTunnel::E_SIZE _eSize, CUnitGenerator* _pUnitGenerator, CCommander* _pCommander = nullptr);
	virtual ~CTunnel();

	virtual int Update(float _fDeltaTime) override;
	virtual void Release(void) override;

private:
	enum E_STATE { STATE_BUILDING, STATE_COMPLETED, STATE_GENERATE_UNIT, STATE_END };

	CUnitGenerator* m_pUnitGenerator;
	CTunnel::E_SIZE m_eSize = CTunnel::SIZE_SMALL;
	CTunnel::E_STATE m_eState = CTunnel::STATE_BUILDING;

	float m_fElapsedTime = 0.f;
};

