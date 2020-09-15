#pragma once
#include "CUI_Image.h"
class CParticle_Blood :
	public CUI_Image
{
public:
	CParticle_Blood(CGameWorld& _rGameWorld, float _fX, float _fY, int _iConcentration);
	virtual ~CParticle_Blood();

public:
	virtual int Update(float _fDeltaTime) override;

public:
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

