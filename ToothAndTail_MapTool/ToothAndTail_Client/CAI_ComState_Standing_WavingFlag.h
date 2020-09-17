#pragma once
#include "CState.h"

class CCommanderAI;
class CAI_ComState_Standing_WavingFlag :
	public CState<CCommanderAI>
{
public:
	CAI_ComState_Standing_WavingFlag(CGameWorld& _rGameWorld, CCommanderAI& _rOwner);
	virtual ~CAI_ComState_Standing_WavingFlag();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	float m_fKeepTime = 0.f;
	float m_fElapsedTime = 0.f;
	bool m_bIsGathering = true;
	float m_fTickTime = 0.f;
	float m_fRepeatTime = 0.f;
};

