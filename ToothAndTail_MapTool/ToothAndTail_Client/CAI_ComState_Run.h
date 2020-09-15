#pragma once
#include "CState.h"

class CCommanderAI;
class CAI_ComState_Run :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Run(CGameWorld& _rGameWorld, CCommanderAI& _rOwner);
	virtual ~CAI_ComState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};
