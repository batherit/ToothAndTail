#include "stdafx.h"
#include "CSkunkState_Idle.h"
#include "CSkunkState_Run.h"
#include "CSkunk.h"
#include "CCommander.h"
#include "CStateMgr.h"

CSkunkState_Idle::CSkunkState_Idle(CGameWorld & _rGameWorld, CSkunk & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSkunkState_Idle::~CSkunkState_Idle()
{
}

void CSkunkState_Idle::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSkunkState_Idle::Update(float _fDeltaTime)
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
				m_rOwner.GetStateMgr()->SetNextState(new CSkunkState_Run(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : Ÿ���� ���� ���� �����Ѵ�.
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSkunkState_Idle::LateUpdate(void)
{
}

void CSkunkState_Idle::OnExited(void)
{
}