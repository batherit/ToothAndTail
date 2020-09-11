#include "stdafx.h"
#include "CBadgerState_Idle.h"
#include "CBadgerState_Run.h"
#include "CBadgerState_Attack.h"
#include "CBadger.h"
#include "CCommander.h"
#include "CStateMgr.h"

CBadgerState_Idle::CBadgerState_Idle(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Idle::~CBadgerState_Idle()
{
}

void CBadgerState_Idle::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CBadgerState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// ��ġ ����(��������.)
		m_rOwner.AdjustPosition(_fDeltaTime);
		if (m_rOwner.GetTargetEnemy()) {
			// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// �ش� �δ뿡�� ����� �������ٸ�,
			// �� Ÿ�� ����Ʈ�� ����
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�. ��ǥ ������ �ִٸ� ���ڸ��� �ְ� �ȴ�.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
		}
		else m_rOwner.AdjustPosition(_fDeltaTime);

		break;
	case COMMANDER::COMMAND_SATURATION:
		// ���߰���.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ����� ������ Ÿ���� ��ȿ�Ѱ�?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// ���� Ÿ���� ����� ������ Ÿ���̴�.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
				if (m_rOwner.CanAttackTargetEnemy()) {
					// ���� ������ �� �ִ� ��Ȳ�̶��, �ٷ� �����Ѵ�.
					m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// ���� ������ �� ���� �Ÿ��� �ִٸ� ���� �ִ� ������ �޷�����.
					m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
				}
			}
			ELSE{
				// ��ȿ���� ���� Ÿ���̸� COMMANDER::COMMAND_GATHERING�� ����.
				// ����� ������ Ÿ���� ��ȿ���� �ʴٸ�, ��� ��ġ�� ��ǥ �������� ��´�.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
			//else

			}
		}

		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Idle::LateUpdate(void)
{
}

void CBadgerState_Idle::OnExited(void)
{
}