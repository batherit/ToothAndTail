#include "stdafx.h"
#include "CLizardState_Run.h"
#include "CLizardState_Idle.h"
#include "CLizard.h"
#include "CStateMgr.h"
#include "CCommander.h"

CLizardState_Run::CLizardState_Run(CGameWorld & _rGameWorld, CLizard & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CLizardState_Run::~CLizardState_Run()
{
}

void CLizardState_Run::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 0.5f, 0, false));
	m_rOwner.SetSpeed(LIZARD_SPEED);
}

int CLizardState_Run::Update(float _fDeltaTime)
{
	// Ŀ����� ����� Ȯ���Ѵ�.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// Ŀ����� ����� ���ٸ�, Ÿ�� �������� �̵��ϸ�, �̵� ���� ���� ������ ���� �ο��.
		// ���� ���� ���� ������ ���� ���� ������� �� ���̴�. ������ �� ������ ������ ������ ���� ��츸 ����Ѵ�.
		// TODO : ���� ���� ���
		// ���� ������ ���� ���
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}



	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
		m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CLizardState_Run::LateUpdate(void)
{
}

void CLizardState_Run::OnExited(void)
{
}
