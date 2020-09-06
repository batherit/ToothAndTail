#include "stdafx.h"
#include "CBadgerState_Run.h"
#include "CBadgerState_Idle.h"
#include "CBadger.h"
#include "CStateMgr.h"
#include "CCommander.h"

CBadgerState_Run::CBadgerState_Run(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Run::~CBadgerState_Run()
{
}

void CBadgerState_Run::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 0.5f, 0, false));
	m_rOwner.SetSpeed(BADGER_SPEED);
}

int CBadgerState_Run::Update(float _fDeltaTime)
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
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}



	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
		m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Run::LateUpdate(void)
{
}

void CBadgerState_Run::OnExited(void)
{
}
