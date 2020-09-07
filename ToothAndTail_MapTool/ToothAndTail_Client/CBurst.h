#pragma once
#include "CSpriteObj.h"
class CBurst :
	public CSpriteObj
{
public:
	CBurst(CGameWorld& _rGameWorld, D3DXVECTOR3 _vStartPos, float _fLifeTime = 1.2f, float _fScale = 1.f);
	virtual ~CBurst();

public:
	virtual int Update(float _fDeltaTime) override;
};

