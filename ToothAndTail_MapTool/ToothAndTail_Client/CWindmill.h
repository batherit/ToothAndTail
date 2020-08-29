#pragma once
#include "CComDepObj.h"

class CCommander;
class CWindmillBase;
class CTurbine;
class CWindmill :
	public CComDepObj
{
public:
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };

public:
	CWindmill(CGameWorld& _rGameWorld, float _fX, float _fY, CWindmill::E_STATE _eState, CCommander* _pCommander = nullptr);
	virtual ~CWindmill();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// 조립 객체를 따로 따로 렌더링 하려는 경우.
	// 혜택. y값 정렬 렌더링
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual CObj* GetCollider(void);

private:
	CWindmill::E_STATE m_eState = STATE_UNOCCUPIED;
	CSpriteObj* m_pGround = nullptr;
	CWindmillBase* m_pWindmillBase = nullptr;
};

