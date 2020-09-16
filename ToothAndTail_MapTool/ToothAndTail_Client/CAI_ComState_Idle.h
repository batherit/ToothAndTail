#pragma once
#include "CState.h"

class CCommanderAI;
class CAI_ComState_Idle :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Idle(CGameWorld& _rGameWorld, CCommanderAI& _rOwner);
	virtual ~CAI_ComState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	float m_fKeepTime = 0.f;
	float m_fElapsedTime = 0.f;
};

