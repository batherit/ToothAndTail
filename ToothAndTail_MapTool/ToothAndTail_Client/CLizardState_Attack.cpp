#include "stdafx.h"
#include "CLizardState_Attack.h"
#include "CLizardState_Idle.h"
#include "CLizardState_Run.h"
#include "CLizard.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CBullet.h"
#include "CCommander.h"


CLizardState_Attack::CLizardState_Attack(CGameWorld & _rGameWorld, CLizard & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CLizardState_Attack::~CLizardState_Attack()
{
}

void CLizardState_Attack::OnLoaded(void)
{
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 10, 1, 0.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CLizardState_Attack::Update(float _fDeltaTime)
{
	// ��� ��� �ؼ�
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// �ϴ� ������ �����Ѵ�.
		//m_fTickTime = 0.f;
	//	m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		break;
	case COMMANDER::COMMAND_GATHERING:
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			if (!m_rOwner.IsLocatedAtTargetPoint())
				// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Run(m_rGameWorld, m_rOwner));
		}


		break;
	case COMMANDER::COMMAND_SATURATION:
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
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
					//if ((m_fTickTime += _fDeltaTime) >= 0.1f) {}
					// ���� ����� �������� ���� 0.1���� �޸��� ���� ��ȯ ���θ� ������.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				}
			}
				// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
				if (!m_rOwner.CanAttackTargetEnemy() && !m_rOwner.IsLocatedAtTargetPoint())
					m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Run(m_rGameWorld, m_rOwner));
		}

		break;
	}

	CComDepObj* pTargetEnemy = m_rOwner.GetTargetEnemy();
	DO_IF_IS_VALID_OBJ(pTargetEnemy) {
		// Ÿ���� ��� �����ǰ� �ִ� ���, �ִϸ��̼��� ��� �����ϸ鼭 �ൿ�Ѵ�.
		D3DXVECTOR3 vToTargetEnemy = pTargetEnemy->GetXY() - m_rOwner.GetXY();
		D3DXVec3Normalize(&vToTargetEnemy, &vToTargetEnemy);
		m_rOwner.SetToXY(vToTargetEnemy);
		m_rOwner.UpdateSpriteDir();

		if (!m_bIsShooting) {
			// 1.0�� ����� źȯ�� �߻��Ѵ�.
			if ((m_fElapsedTime += _fDeltaTime) >= LIZARD_ATTACK_DELAY) {
				// �Ѿ� ����
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_LIZARD, m_rOwner.GetXY()));
				// �߻� �ִϸ��̼��� �����Ѵ�.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 11, 7, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// �ִϸ��̼��� ������ �ϴ� ���� ���·� �ٽ� �������Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Attack(m_rGameWorld, m_rOwner));
				return 1;
			}
		}
	}
	ELSE{
		// ������ ���� ���ٸ�, ��ǥ ������ ��ġ���ִ����� ���� �޸� ������ �� �������� �����Ѵ�.
		if (!m_rOwner.IsLocatedAtTargetPoint())
			m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Run(m_rGameWorld, m_rOwner));
		else
			m_rOwner.GetStateMgr()->SetNextState(new CLizardState_Idle(m_rGameWorld, m_rOwner));
	}

	return 0;
}

void CLizardState_Attack::LateUpdate(void)
{
}

void CLizardState_Attack::OnExited(void)
{
}