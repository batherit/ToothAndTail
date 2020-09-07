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
	//CWindmill(CGameWorld& _rGameWorld, float _fX, float _fY, CCommander* _pCommander = nullptr);
	CWindmill(CGameWorld& _rGameWorld, int _iLineIndex, CCommander* _pCommander = nullptr);
	virtual ~CWindmill();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// 조립 객체를 따로 따로 렌더링 하려는 경우.
	// 혜택. y값 정렬 렌더링
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual void InvalidateObj() override;
	void Occupied(CCommander* _pCommander);
	//virtual void CollectGarbageObjs();
	//vector<CFarmland*>& GetFarmlands() { return m_vecFarmlands; }

public:
	//void SetWindmillState(CWindmill::E_STATE _eState) { m_eState = _eState; }
	//CWindmill::E_STATE GetWindmillState() const { return m_eState; }
	WINDMILL::E_STATE GetState() const;
	

private:
	//CWindmill::E_STATE m_eState = STATE_UNOCCUPIED;
	CSpriteObj* m_pGround = nullptr;
	vector<CFarmland*> m_vecFarmlands;
	CWindmillBase* m_pWindmillBase = nullptr;
};

