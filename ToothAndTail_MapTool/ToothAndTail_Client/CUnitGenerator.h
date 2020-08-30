#pragma once
class CGameWorld;
class CUnitGenerator
{
public:
	CUnitGenerator(CGameWorld& _rGameWorld);
	virtual ~CUnitGenerator();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Release(void);

protected:
	CGameWorld& m_rGameWorld;
	float m_fElapedTime = 0.f;
};

