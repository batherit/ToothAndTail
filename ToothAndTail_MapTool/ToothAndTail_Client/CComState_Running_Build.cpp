#include "stdafx.h"
#include "CComState_Running_Build.h"
#include "CComState_Running_WaveFlag.h"
#include "CComState_Standing_Build.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Idle.h"
#include "CComState_Run.h"
#include "CCommander.h"
#include "CStateMgr.h"



CComState_Running_Build::CComState_Running_Build(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
	m_wstrBuildText[0] = L"더 이상 땅굴을 설치할 수 없구만,,@ㅁ@!!";
	m_wstrBuildText[1] = L"제분소 당 12개의 땅굴을 설치할 수 있어,,,!!";
	m_wstrBuildText[2] = L"쥐들의 욕심은 끝이 없고,,,";
}

CComState_Running_Build::~CComState_Running_Build()
{
}

void CComState_Running_Build::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 42, 5, 0.4f, 1, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
	if (!m_rOwner.GenerateTunnel()) {
		if(m_rOwner.GetTotalTunnelsNum() >= m_rOwner.GetInstallableTunnelNum())
			m_rOwner.ShoutOut(-150.f, -120.f, m_wstrBuildText[rand() % 3]);
	}
}

int CComState_Running_Build::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	//CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (!m_rOwner.IsAnimPlaying()) {
			if (m_rOwner.IsWavingFlag(/*eFlagType*/)) 
				m_rOwner.GetStateMgr()->SetNextState(new CComState_Running_WaveFlag(m_rGameWorld, m_rOwner));
			else 
				m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
		}
	}
	/*else if (m_rOwner.IsBuildKeyPressed()) {
		if (!m_rOwner.IsAnimPlaying())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsFlagKeyPressed(eFlagType)) {
		if (!m_rOwner.IsAnimPlaying())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_WaveFlag(m_rGameWorld, m_rOwner));
	}*/
	else {
		if (!m_rOwner.IsAnimPlaying())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Running_Build::LateUpdate(void)
{
}

void CComState_Running_Build::OnExited(void)
{
}
