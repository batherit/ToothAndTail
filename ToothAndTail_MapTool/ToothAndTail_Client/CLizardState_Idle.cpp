#include "stdafx.h"
#include "CLizardState_Idle.h"
#include "CLizardState_Run.h"
#include "CLizard.h"
#include "CCommander.h"
#include "CStateMgr.h"

CLizardState_Idle::CLizardState_Idle(CGameWorld & _rGameWorld, CLizard & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CLizardState_Idle::~CLizardState_Idle()
{
}

void CLizardState_Idle::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CLizardState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// TODO : �ƹ��͵� ���� �ʴ´�.
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);

			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Run(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CLizardState_Idle::LateUpdate(void)
{
}

void CLizardState_Idle::OnExited(void)
{
}
