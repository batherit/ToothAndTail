#pragma once
#include "CComDepObj.h"

class CCommander;
class CFarmland :
	public CComDepObj
{
public:
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };

public:
	CFarmland(CGameWorld& _rGameWorld, float _fX, float _fY, CFarmland::E_STATE _eState, CCommander* _pCommander = nullptr);
	virtual ~CFarmland();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);

private:
	CFarmland::E_STATE m_eState = STATE_UNOCCUPIED;
};

