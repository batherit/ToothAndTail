#pragma once
#include "CState.h"
class CCommanderAI;
class CAI_ComState_Running_Gathering :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Running_Gathering(CGameWorld& _rGameWorld, CCommanderAI& _rOwner);
	virtual ~CAI_ComState_Running_Gathering();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsGathering = true;
	float m_fTickTime = 0.f;
	float m_fRepeatTime = 0.f;
};

