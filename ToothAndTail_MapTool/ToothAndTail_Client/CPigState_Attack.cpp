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
		// 타겟이 계속 유지되고 있는 경우, 애니메이션을 계속 진행하면서 행동한다.
		D3DXVECTOR3 vToTargetEnemy = pTargetEnemy->GetXY() - m_rOwner.GetXY();
		D3DXVec3Normalize(&vToTargetEnemy, &vToTargetEnemy);
		m_rOwner.SetToXY(vToTargetEnemy);
		m_rOwner.UpdateSpriteDir();

		if (!m_bIsShooting) {
			// 1.5초 대기후 탄환을 발사한다.
			if ((m_fElapsedTime += _fDeltaTime) >= PIG_ATTACK_DELAY) {
				// 총알 생성
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_PIG, m_rOwner.GetXY()));
				// 발사 애니메이션을 진행한다.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 26, 3, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// 총알 발사 애니메이션이 끝나면 새로운 적을 탐색한다.
				m_rOwner.DetectUnitsAround();
				if (m_rOwner.GetTargetEnemy()) {
					// 주변에 적을 감지했다면, 공격 상태로 전환한다.
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
