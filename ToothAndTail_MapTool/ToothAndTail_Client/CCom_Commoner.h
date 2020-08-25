#pragma once
#include "CSpriteObj.h"
class CCom_Commoner :
	public CSpriteObj
{
public:
	CCom_Commoner(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CCom_Commoner();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;
};

