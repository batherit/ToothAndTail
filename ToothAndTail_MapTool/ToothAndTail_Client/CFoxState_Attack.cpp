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
	// 기수 명령 해석
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 하던 공격을 마저한다.
		// 추가
		if (m_rOwner.GetTargetEnemy()) {
			if (!m_rOwner.CanAttackTargetEnemy()) {
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
			}
		}
		break;
	case COMMANDER::COMMAND_GATHERING:
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			if (!m_rOwner.IsLocatedAtTargetPoint())
				// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
		}


		break;
	case COMMANDER::COMMAND_SATURATION:
		//m_rOwner.AdjustPosition(_fDeltaTime, GetNumberMinBetweenMax(0.f, 1.f));
		// 집중공격.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 기수가 선정한 타겟이 유효한가?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// 공격 타겟은 기수가 선정한 타겟이다.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
			}
			ELSE{
				if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
					//if ((m_fTickTime += _fDeltaTime) >= 0.1f) {}
					// 집합 명령이 내려지고 나서 0.1초후 달리기 상태 전환 여부를 따진다.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				}
			}
				// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
				if (!m_rOwner.CanAttackTargetEnemy() && !m_rOwner.IsLocatedAtTargetPoint())
					m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
		}

		break;
	}

	CComDepObj* pTargetEnemy = m_rOwner.GetTargetEnemy();
	DO_IF_IS_VALID_OBJ(pTargetEnemy) {
		// 타겟이 계속 유지되고 있는 경우, 애니메이션을 계속 진행하면서 행동한다.
		D3DXVECTOR3 vToTargetEnemy = pTargetEnemy->GetXY() - m_rOwner.GetXY();
		D3DXVec3Normalize(&vToTargetEnemy, &vToTargetEnemy);
		m_rOwner.SetToXY(vToTargetEnemy);
		m_rOwner.UpdateSpriteDir();

		if (!m_bIsShooting) {
			// 1.0초 대기후 탄환을 발사한다.
			if ((m_fElapsedTime += _fDeltaTime) >= FOX_ATTACK_DELAY + m_fAttackDelayOffset) {
				// 총알 생성
				m_rGameWorld.GetListObjs().emplace_back(new CBullet(m_rGameWorld, pTargetEnemy, UNIT::TYPE_FOX, m_rOwner.GetXY()));
				// 발사 애니메이션을 진행한다.
				m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 17, 4, 0.2f, 1, false));
				m_bIsShooting = true;
			}
		}
		else {
			if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
				// 애니메이션이 끝나면 일단 공격 상태로 다시 재진입한다.
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
				return 1;
			}
		}
	}
	ELSE{
		// 감지된 적이 없다면, 목표 지점에 위치해있는지에 따라 달릴 것인지 말 것인지를 결정한다.
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