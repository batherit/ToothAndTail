#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CCommander;
class CTunnel;
class CSkunk :
	public CComDepObj
{
public:
	CSkunk(CGameWorld& _rGameWorld, CCommander* _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID = -1);
	virtual ~CSkunk();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Release() override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);

public:
	CStateMgr<CSkunk>* GetStateMgr(void) const { return m_pStateMgr; }

private:
	CTunnel* m_pTunnel = nullptr;		// ������ ���޼��� �����ϴµ� ���δ�. Ex ������ ���� �� 1 ����
	CStateMgr<CSkunk>* m_pStateMgr = nullptr;
};

