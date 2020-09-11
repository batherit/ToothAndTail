#include "stdafx.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrelState_Run.h"
#include "CSquirrelState_Attack.h"
#include "CSquirrel.h"
#include "CStateMgr.h"
#include "CCommander.h"

CSquirrelState_Idle::CSquirrelState_Idle(CGameWorld & _rGameWorld, CSquirrel & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSquirrelState_Idle::~CSquirrelState_Idle()
{
}

void CSquirrelState_Idle::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSquirrelState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// ��ġ ����(��������.)
		m_rOwner.AdjustPosition(_fDeltaTime);
		if (m_rOwner.GetTargetEnemy()) {
			// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
			m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// �ش� �δ뿡�� ����� �������ٸ�,
			// �� Ÿ�� ����Ʈ�� ����
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);	
			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�. ��ǥ ������ �ִٸ� ���ڸ��� �ְ� �ȴ�.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
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
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// ���� ������ �� ���� �Ÿ��� �ִٸ� ���� �ִ� ������ �޷�����.
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
				}
			}
			ELSE {
			// ��ȿ���� ���� Ÿ���̸� COMMANDER::COMMAND_GATHERING�� ����.
			// ����� ������ Ÿ���� ��ȿ���� �ʴٸ�, ��� ��ġ�� ��ǥ �������� ��´�.
				m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
				if (!m_rOwner.IsLocatedAtTargetPoint())
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
				//else
					
			}
		}
		
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Idle::LateUpdate(void)
{
}

void CSquirrelState_Idle::OnExited(void)
{
}
