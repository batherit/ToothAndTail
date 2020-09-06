#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CCommander;
class CTunnel;
class CFox :
	public CComDepObj
{
public:
	CFox(CGameWorld& _rGameWorld, CCommander* _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID = -1);
	virtual ~CFox();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;

public:
	CStateMgr<CFox>* GetStateMgr(void) const { return m_pStateMgr; }

private:
	CTunnel* m_pTunnel = nullptr;		// ������ ���޼��� �����ϴµ� ���δ�. Ex ������ ���� �� 1 ����
	CStateMgr<CFox>* m_pStateMgr = nullptr;
};

