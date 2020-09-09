#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CCommander;
class CTunnel;
class CSquirrel :
	public CComDepObj
{
public:
	CSquirrel(CGameWorld& _rGameWorld, CCommander* _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID = -1);
	virtual ~CSquirrel();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Release() override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);

public:
	CStateMgr<CSquirrel>* GetStateMgr(void) const { return m_pStateMgr; }

private:					// 커멘더로부터 부대 명령을 받을 때 사용한다.
	CTunnel* m_pTunnel = nullptr;		// 땅굴의 보급수를 조정하는데 쓰인다. Ex 죽으면 보급 수 1 감소
	CStateMgr<CSquirrel>* m_pStateMgr = nullptr;
};

