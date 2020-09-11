#include "stdafx.h"
#include "CPigState_Attack.h"
#include "CPigState_Patrol.h"
#include "CPig.h"
#include "CStateMgr.h"
#include "CGameWorld.h"
#include "CBullet.h"


CPigState_Attack::CPigState_Attack(CGameWorld & _rGameWorld, CPig & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CPigState_Attack::~CPigState_Attack()
{
}

void CPigState_Attack::OnLoaded(void)
{ 
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 25, 1, 0.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CPigState_Attack::Update(float _fDeltaTime)
{
	CComDepObj* pTargetEnemy = m_rOwner.GetTargetEnemy();
	DO_IF_IS_VALID_OBJ(pTargetEnemy) {
		// Ÿ���� ��� �����ǰ� �ִ� ���, �ִϸ��̼��� ��� �����ϸ鼭 �ൿ�Ѵ�.
		D3DXVECTOR3 vToTargetEnemy = pTargetEnemy->GetXY() - m_rOwner.GetXY();
		D3DXVec3Normalize(&vToTargetEnemy, &vToTargetEnemy);
		m_rOwner.SetToXY(vToTargetEnemy);
		m_rOwner.UpdateSpriteDir();

		if (!m_bIsShooting) {
			// 1.5�� ����� źȯ�� �߻��Ѵ�.
			if ((m_fElapsedTime += _fDeltaTime) >= PIG_ATTACK_DELAY) {
				// �Ѿ� ����
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_PIG, m_rOwner.GetXY()));
				// �߻� �ִϸ��̼��� �����Ѵ�.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 26, 3, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// �Ѿ� �߻� �ִϸ��̼��� ������ ���ο� ���� Ž���Ѵ�.
				m_rOwner.DetectUnitsAround();
				if (m_rOwner.GetTargetEnemy()) {
					// �ֺ��� ���� �����ߴٸ�, ���� ���·� ��ȯ�Ѵ�.
					m_rOwner.GetStateMgr()->SetNextState(new CPigState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					m_rOwner.GetStateMgr()->SetNextState(new CPigState_Patrol(m_rGameWorld, m_rOwner));
				}
				return 1;
			}
		}
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CPigState_Patrol(m_rGameWorld, m_rOwner));
	}

	return 0;
}

void CPigState_Attack::LateUpdate(void)
{
}

void CPigState_Attack::OnExited(void)
{
}
