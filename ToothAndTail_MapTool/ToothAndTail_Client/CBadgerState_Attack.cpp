#include "stdafx.h"
#include "CBadgerState_Attack.h"
#include "CBadgerState_Idle.h"
#include "CBadgerState_Run.h"
#include "CBadger.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CBullet.h"
#include "CCommander.h"


CBadgerState_Attack::CBadgerState_Attack(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Attack::~CBadgerState_Attack()
{
}

void CBadgerState_Attack::OnLoaded(void)
{
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 14, 1, 0.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CBadgerState_Attack::Update(float _fDeltaTime)
{
	// ��� ��� �ؼ�
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// �ϴ� ������ �����Ѵ�.
		//m_fTickTime = 0.f;
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			//if ((m_fTickTime += _fDeltaTime) >= 0.1f) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
			else
				m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
			//}
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// ���߰���.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// ����� ������ Ÿ���� ��ȿ�Ѱ�?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// ���� Ÿ���� ����� ������ Ÿ���̴�.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
				if (!m_rOwner.CanAttackTargetEnemy()) {
					// ���� ������ �� ���� �Ÿ��� �ִٸ� ���� �ִ� ������ �޷�����.
					m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
				}
			}
			ELSE{
				if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
					//if ((m_fTickTime += _fDeltaTime) >= 0.1f) {}
					// ���� ����� �������� ���� 0.1���� �޸��� ���� ��ȯ ���θ� ������.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
					// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
					if (!m_rOwner.IsLocatedAtTargetPoint())
						m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
					else
						m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
					}
			}
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
			if ((m_fElapsedTime += _fDeltaTime) >= SQUIRREL_ATTACK_DELAY) {
				// �Ѿ� ����
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_BADGER, m_rOwner.GetXY()));
				// �߻� �ִϸ��̼��� �����Ѵ�.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 15, 3, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// �Ѿ� �߻� �ִϸ��̼��� ������ ���ο� ���� Ž���Ѵ�.
				m_rOwner.DetectUnitsAround();
				if (m_rOwner.GetTargetEnemy()) {
					// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
					m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// ������ ���� ���ٸ�, ��ǥ ������ ��ġ���ִ����� ���� �޸� ������ �� �������� �����Ѵ�.
					if (!m_rOwner.IsLocatedAtTargetPoint())
						m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
					else
						m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
				}
				return 1;
			}
		}
	}
	else {
	// ������ ���� ���ٸ�, ��ǥ ������ ��ġ���ִ����� ���� �޸� ������ �� �������� �����Ѵ�.
	if (!m_rOwner.IsLocatedAtTargetPoint())
		m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
	else
		m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
	}

	return 0;
}

void CBadgerState_Attack::LateUpdate(void)
{
}

void CBadgerState_Attack::OnExited(void)
{
}