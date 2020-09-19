#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CCommander;
class CTunnel;
class CTunnelGenerator;
class CFox :
	public CComDepObj
{
public:
	CFox(CGameWorld& _rGameWorld, CCommander* _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID = -1);
	virtual ~CFox();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Release() override;
	virtual void InvalidateObj(void) override;
	virtual void ReleaseTunnel(CTunnel* _pTunnel) {
		if (m_pTunnel != _pTunnel) return;
		m_pTunnel = nullptr;
	}
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void TakeDamage(float _fDamageAmount, CObj* _pAttacker) override;
public:
	CStateMgr<CFox>* GetStateMgr(void) const { return m_pStateMgr; }

private:
	CTunnelGenerator* m_pTunnelGenerator = nullptr;
	CTunnel* m_pTunnel = nullptr;		// 땅굴의 보급수를 조정하는데 쓰인다. Ex 죽으면 보급 수 1 감소
	CStateMgr<CFox>* m_pStateMgr = nullptr;
};

