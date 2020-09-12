#include "stdafx.h"
#include "CMoleState_Run.h"
#include "CMoleState_Idle.h"
#include "CMoleState_Attack.h"
#include "CMole.h"
#include "CStateMgr.h"
#include "CCommander.h"

CMoleState_Run::CMoleState_Run(CGameWorld & _rGameWorld, CMole & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CMoleState_Run::~CMoleState_Run()
{
}

void CMoleState_Run::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 8, 0.5f, 0, false));
	m_rOwner.SetSpeed(MOLE_SPEED);
}

int CMoleState_Run::Update(float _fDeltaTime)
{
	// Ŀ����� ����� Ȯ���Ѵ�.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		if (m_rOwner.GetTargetEnemy()) {
			if (m_rOwner.CanAttackTargetEnemy()) {
				// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Attack(m_rGameWorld, m_rOwner));
			}
			else {
				// �ֺ��� ���� ����������, ���� ���� �ۿ� ������ Ÿ�������� �޷�����.
				m_rOwner.GoToTarget(_fDeltaTime);
			}
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ��ɿ� �ش��ϴ� ������ ���ο� ��ǥ������ �����Ѵ�.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		else if (m_rOwner.GetTargetEnemy()) {
			if (m_rOwner.CanAttackTargetEnemy()) {
				m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Attack(m_rGameWorld, m_rOwner));
			}
		}
		// ��ǥ �������� �޸��⸦ �����Ѵ�.
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
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
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Attack(m_rGameWorld, m_rOwner));
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
		}
		break;

		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CMoleState_Run::LateUpdate(void)
{
}

void CMoleState_Run::OnExited(void)
{
}
