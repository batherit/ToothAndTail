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
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual void InvalidateObj() override;
	void Occupied(CCommander* _pCommander);
	virtual void TakeDamage(float _fDamageAmount, CObj* _pAttacker) override;
	D3DXVECTOR3 GetAttackerPos() const { return m_vAttackerPos; }

public:
	//void SetWindmillState(CWindmill::E_STATE _eState) { m_eState = _eState; }
	//CWindmill::E_STATE GetWindmillState() const { return m_eState; }
	WINDMILL::E_STATE GetState() const;
	vector<CFarmland*>& GetFarmlands() { return m_vecFarmlands; }
	bool DetectEmptyLot(D3DXVECTOR3& _rEmptyLotPos);
	bool IsAttackedRecently() const { return m_bIsAttackedRecently; }
	
private:
	//CWindmill::E_STATE m_eState = STATE_UNOCCUPIED;
	
	CSpriteObj* m_pGround = nullptr;
	vector<CFarmland*> m_vecFarmlands;
	float m_fCroppingTickTime = 0.f;
	CWindmillBase* m_pWindmillBase = nullptr;

	bool m_bIsAttackedRecently = false;
	float m_fAttackedTickTime = 0.f;
	D3DXVECTOR3 m_vAttackerPos;
};

