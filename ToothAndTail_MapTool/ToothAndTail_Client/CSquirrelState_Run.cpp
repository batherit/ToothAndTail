#include "stdafx.h"
#include "CSquirrelState_Run.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrel.h"
#include "CStateMgr.h"

CSquirrelState_Run::CSquirrelState_Run(CGameWorld & _rGameWorld, CSquirrel & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSquirrelState_Run::~CSquirrelState_Run()
{
}

void CSquirrelState_Run::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 1.f, 0, false));
	m_rOwner.SetSpeed(SQUIRREL_SPEED);
}

int CSquirrelState_Run::Update(float _fDeltaTime)
{
	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // �̵��� �����ϴ� => ��ǥ������ �����ߴ�, �� ���� ����.
		m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Run::LateUpdate(void)
{
}

void CSquirrelState_Run::OnExited(void)
{
}
