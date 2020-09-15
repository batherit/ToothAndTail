#include "stdafx.h"
#include "CFoxState_Attack.h"
#include "CFoxState_Idle.h"
#include "CFoxState_Run.h"
#include "CFox.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CBullet.h"
#include "CCommander.h"


CFoxState_Attack::CFoxState_Attack(CGameWorld & _rGameWorld, CFox & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CFoxState_Attack::~CFoxState_Attack()
{
}

void CFoxState_Attack::OnLoaded(void)
{
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 16, 1, 0.f, 0, false));
	m_rOwner.SetSpeed(0.f);
	m_fAttackDelayOffset = GetNumberMinBetweenMax(0.f, 0.8f);
	if (IsObjInCamera(&m_rOwner, m_rGameWorld.GetMainCamera()))
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Fox_Reload.wav", CSoundMgr::EFFECT);
}

int CFoxState_Attack::Update(float _fDeltaTime)
{
	// ��� ��� �ؼ�
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// �ϴ� ������ �����Ѵ�.
		//m_fTickTime = 0.f;
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		break;
	case COMMANDER::COMMAND_GATHERING:
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			if (!m_rOwner.IsLocatedAtTargetPoint())
				// ��ǥ ������ ��ġ������ �ʴٸ� �޸��� ���·� �����Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
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
					m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
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
			if ((m_fElapsedTime += _fDeltaTime) >= FOX_ATTACK_DELAY + m_fAttackDelayOffset) {
				// �Ѿ� ����
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_FOX, m_rOwner.GetXY()));
				// �߻� �ִϸ��̼��� �����Ѵ�.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 17, 4, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// �ִϸ��̼��� ������ �ϴ� ���� ���·� �ٽ� �������Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
				return 1;
			}
		}
	}
	ELSE{
		// ������ ���� ���ٸ�, ��ǥ ������ ��ġ���ִ����� ���� �޸� ������ �� �������� �����Ѵ�.
		if (!m_rOwner.IsLocatedAtTargetPoint())
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
		else
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Idle(m_rGameWorld, m_rOwner));
	}

	return 0;
}

void CFoxState_Attack::LateUpdate(void)
{
}

void CFoxState_Attack::OnExited(void)
{
}