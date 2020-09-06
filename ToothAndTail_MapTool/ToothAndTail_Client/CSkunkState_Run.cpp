#include "stdafx.h"
#include "CSkunkState_Run.h"
#include "CSkunkState_Idle.h"
#include "CSkunk.h"
#include "CStateMgr.h"
#include "CCommander.h"

CSkunkState_Run::CSkunkState_Run(CGameWorld & _rGameWorld, CSkunk & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSkunkState_Run::~CSkunkState_Run()
{
}

void CSkunkState_Run::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 0.5f, 0, false));
	m_rOwner.SetSpeed(SKUNK_SPEED);
}

int CSkunkState_Run::Update(float _fDeltaTime)
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
			m_rOwner.GetStateMgr()->SetNextState(new CSkunkState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CSkunkState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}



	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
		m_rOwner.GetStateMgr()->SetNextState(new CSkunkState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSkunkState_Run::LateUpdate(void)
{
}

void CSkunkState_Run::OnExited(void)
{
}
