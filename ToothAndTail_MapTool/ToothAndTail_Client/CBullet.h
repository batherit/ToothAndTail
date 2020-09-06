#pragma once
#include "CSpriteObj.h"

class CComDepObj;
class CBullet :
	public CSpriteObj
{
public:
	CBullet(CGameWorld& _rGameWorld, CComDepObj* _pTarget, UNIT::E_TYPE _eUnitType, D3DXVECTOR3 _vStartPos);
	virtual ~CBullet();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void CollectGarbageObjs() override;

private:
	CComDepObj* m_pTarget = nullptr;
	D3DXVECTOR3 m_vTargetPos;
	float m_fDamage = 0.f;
};

