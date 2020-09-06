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
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// TODO : �ƹ��͵� ���� �ʴ´�.
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);

			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
			if(!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}

	// ���� �ִ����� ã�ƺ���.
	m_rOwner.DetectEnemyAround();
	if (m_rOwner.GetTargetEnemy()) {
		// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
		m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Idle::LateUpdate(void)
{
}

void CSquirrelState_Idle::OnExited(void)
{
}
