#pragma once
#include "CState.h"

class CCommanderAI;
class CAI_ComState_Run :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Run(CGameWorld& _rGameWorld, CCommanderAI& _rOwner, bool _bIsActivating);
	virtual ~CAI_ComState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsActivating = false;
};

