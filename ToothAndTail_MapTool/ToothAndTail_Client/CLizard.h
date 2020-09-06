#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CCommander;
class CTunnel;
class CLizard :
	public CComDepObj
{
public:
	CLizard(CGameWorld& _rGameWorld, CCommander* _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID = -1);
	virtual ~CLizard();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Release() override;

public:
	CStateMgr<CLizard>* GetStateMgr(void) const { return m_pStateMgr; }

private:
	CTunnel* m_pTunnel = nullptr;		// 땅굴의 보급수를 조정하는데 쓰인다. Ex 죽으면 보급 수 1 감소
	CStateMgr<CLizard>* m_pStateMgr = nullptr;
};

