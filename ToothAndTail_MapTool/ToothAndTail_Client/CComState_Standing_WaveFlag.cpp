#include "stdafx.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Standing_Build.h"
#include "CComState_Idle.h"
#include "CComState_Run.h"
#include "CCommander.h"
#include "CStateMgr.h"


CComState_Standing_WaveFlag::CComState_Standing_WaveFlag(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
	m_wstrTroopGathering[0] = L"�δ� ����!!";
	m_wstrTroopGathering[1] = L"�ڳ׵��� �� ������ð�,,!";
	m_wstrTroopGathering[2] = L"���θ��Գ�,,!!";
	m_wstrArmyGathering[0] = L"������, ��� ����� ���̽ÿ�,,!!";
	m_wstrArmyGathering[1] = L"������ ���۰� ���� �̰����� �Ͼ�����,,!!";
	m_wstrArmyGathering[2] = L"���δ�, ��� �̰����� �����Ͻÿ�!!";
	m_wstrSaturation[0] = L"���� �����Ͻÿ�!!";
	m_wstrSaturation[1] = L"ȭ���� �����Ͻÿ�!!";
	m_wstrSaturation[2] = L"�μ��� �����Ͻÿ�!!";
}

CComState_Standing_WaveFlag::~CComState_Standing_WaveFlag()
{
}

void CComState_Standing_WaveFlag::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 10, 10, 1.f, 0, false); // ���� �ִ� �׽�Ʈ
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CComState_Standing_WaveFlag::Update(float _fDeltaTime)
{
	CommandInfo& rCommandInfo = m_rOwner.GetCurrentCommandInfo();

	if ((m_fShoutOutTime -= _fDeltaTime) <= 0.f || m_eCommand != rCommandInfo.eCommand) {
		switch (rCommandInfo.eCommand)
		{
		case COMMANDER::COMMAND_GATHERING:
			if (rCommandInfo.iUnitID == -1 && m_rOwner.GetTotalUnitsNum() > 0) {
				// ���� ����
				m_rOwner.ShoutOut(-100.f, -120.f, m_wstrArmyGathering[rand() % 3]);
				m_fShoutOutTime = 3.f;
				break;
			}
			else if (m_rOwner.GetSelectedUnitsNum() > 0) {
				m_rOwner.ShoutOut(-100.f, -120.f, m_wstrTroopGathering[rand() % 3]);
				m_fShoutOutTime = 3.f;
				break;
			}
			m_fShoutOutTime = 0.f;
			break;
		case COMMANDER::COMMAND_SATURATION:
			if (m_rOwner.GetTotalUnitsNum() > 0) {
				m_rOwner.ShoutOut(-100.f, -120.f, m_wstrSaturation[rand() % 3]);
				m_fShoutOutTime = 3.f;
				break;
			}
			m_fShoutOutTime = 0.f;
			break;
		default:
			break;
		}
	}
	m_eCommand = rCommandInfo.eCommand;

	float fNewToX = 0.f;
	float fNewToY = 0.f;
	//CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsActivating())
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	else if(!m_rOwner.IsWavingFlag(/*eFlagType*/)) 
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Standing_WaveFlag::LateUpdate(void)
{
}

void CComState_Standing_WaveFlag::OnExited(void)
{
}
