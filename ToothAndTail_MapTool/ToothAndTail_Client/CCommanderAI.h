#pragma once
#include "CCommander.h"


class CGameWorld;
class CPathGenerator;
class CTile;
class CWindmill;
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
	CStateMgr<CCommanderAI>* GetStateMgr(void) const { return m_pStateMgr; }
	virtual bool IsMoving(float & _fToX, float & _fToY);		// AI������ ������
	virtual bool IsBuilding() const;							// AI������ ������
	virtual bool IsOccupying() const;

public:
	//bool DetectPlaceToGo();
	//bool DetectEmptyLotToGoAroundWindmill(D3DXVECTOR3& _rGoalPos, const CWindmill* _pWindmill);
	vector<CWindmill*>& ExtractWindmills(WINDMILL::E_OWN_TYPE _eOwnType);
	bool GeneratePathToGoal(const D3DXVECTOR3 & _vGoalPos, CWindmill* _pTargetWindmill);
	bool MoveAlongPath(float _fDeltaTime);
	//void SetTargetWindmill(CWindmill* _pTargetWindmill) { m_pTargetWindmill = _pTargetWindmill; }
	CWindmill* GetTargetWindmill() const { return m_pTargetWindmill; }

private:
	bool DetectWindmills();

private:
	//WINDMILL::E_OWN_TYPE _eOwnType = WINDMILL::TYPE_OWN;
	//CWindmill* m_pDetectedWindmill = nullptr;
	CWindmill* m_pTargetWindmill = nullptr;
	vector<CWindmill*> m_vecExtractedWindmills;
	vector<CWindmill*> m_vecWindmills;
	CStateMgr<CCommanderAI>* m_pStateMgr = nullptr;
	CPathGenerator* m_pPathGenerator = nullptr;
};

