#pragma once
#include "CState.h"
class CCommanderAI;
class CAI_ComState_Standing_Action :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Standing_Action(CGameWorld& _rGameWorld, CCommanderAI& _rOwner);
	virtual ~CAI_ComState_Standing_Action();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

