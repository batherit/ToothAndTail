#pragma once
#include "CState.h"

class CCommander;
class CComState_Running_WaveFlag :
	public CState<CCommander>
{
public:
	CComState_Running_WaveFlag(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Running_WaveFlag();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	wstring m_wstrTroopGathering[3];
	wstring m_wstrArmyGathering[3];
	wstring m_wstrSaturation[3];
	COMMANDER::E_COMMAND m_eCommand = COMMANDER::COMMAND_NOTHING;
	float m_fShoutOutTime = 0.f;
};

