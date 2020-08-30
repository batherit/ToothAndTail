#include "stdafx.h"
#include "CPigState_Attack.h"


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
}

int CPigState_Attack::Update(float _fDeltaTime)
{
	return 0;
}

void CPigState_Attack::LateUpdate(void)
{
}

void CPigState_Attack::OnExited(void)
{
}
