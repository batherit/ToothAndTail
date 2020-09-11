#include "stdafx.h"
#include "CBadgerState_Run.h"
#include "CBadgerState_Idle.h"
#include "CBadgerState_Attack.h"
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
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 10, 0.5f, 0, false));
	m_rOwner.SetSpeed(BADGER_SPEED);
}

int CBadgerState_Run::Update(float _fDeltaTime)
{
	// Ŀ����� ����� Ȯ���Ѵ�.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		if (m_rOwner.GetTargetEnemy()) {
			// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // ���� �������� ���߰�, ��ǥ ������ �������� ���ߴٸ� �̵��� �����Ѵ�.
			// �̵� false => ��ǥ������ �����ߴ�
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		else if (m_rOwner.GetTargetEnemy()) {
			// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
		}
		// ��ǥ �������� �޸��⸦ �����Ѵ�.
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		// ���߰���.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ����� ������ Ÿ���� ��ȿ�Ѱ�?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// ���� Ÿ���� ����� ������ Ÿ���̴�.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
			}
			ELSE{
				if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
					// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				}
			}
		}
		if (m_rOwner.CanAttackTargetEnemy()) {
			// ���� ������ �� �ִ� ��Ȳ�̶��, �ٷ� �����Ѵ�.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Run::LateUpdate(void)
{
}

void CBadgerState_Run::OnExited(void)
{
}
