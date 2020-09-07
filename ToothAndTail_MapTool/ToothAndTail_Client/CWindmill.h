#pragma once
#include "CComDepObj.h"

class CCommander;
class CWindmillBase;
class CTurbine;
class CFarmland;

class CWindmill :
	public CComDepObj
{
public:
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };

public:
	CWindmill(CGameWorld& _rGameWorld, float _fX, float _fY, CCommander* _pCommander = nullptr);
	CWindmill(CGameWorld& _rGameWorld, int _iLineIndex, CCommander* _pCommander = nullptr);
	virtual ~CWindmill();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual void InvalidateObj() override;
	//virtual void CollectGarbageObjs();

private:
	CWindmill::E_STATE m_eState = STATE_UNOCCUPIED;
	CSpriteObj* m_pGround = nullptr;
	vector<CFarmland*> m_vecFarmlands;
	CWindmillBase* m_pWindmillBase = nullptr;
};

