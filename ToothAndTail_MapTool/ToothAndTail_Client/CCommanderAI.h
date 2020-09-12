#pragma once
#include "CCommander.h"


class CGameWorld;
class CPathGenerator;
class CTile;
class CCommanderAI :
	public CCommander
{
public:
	CCommanderAI(CGameWorld& _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType = COM_TYPE_END, D3DCOLOR _clIdentificationTint_ARGB = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual ~CCommanderAI();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

public:
	bool DetectPlaceToGo();
	bool DetectPlaceToGoAroundWindmill(D3DXVECTOR3& _rGoalPos, const CWindmill* _pWindmill);
	bool MoveAlongPath(float _fDeltaTime);
	CStateMgr<CCommanderAI>* GetStateMgr(void) const { return m_pStateMgr; }
	virtual bool IsMoving(float & _fToX, float & _fToY);		// AI측에서 재정의
	virtual bool IsBuilding() const;							// AI측에서 재정의
	virtual bool IsOccupying() const;

private:
	int m_iOldWindmillIndex = -1;
	vector<CWindmill*> m_vecWindmills;
	list<CTile*> m_listPath;
	CStateMgr<CCommanderAI>* m_pStateMgr = nullptr;
	CPathGenerator* m_pPathGenerator = nullptr;
};

