#pragma once
#include "CComDepObj.h"

class CCommander;
class CPig;
class CCrop;
class CFarmland :
	public CComDepObj
{
public:
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };

public:
	CFarmland(CGameWorld& _rGameWorld, float _fX, float _fY, CFarmland::E_STATE _eState = CFarmland::STATE_UNOCCUPIED, CCommander* _pCommander = nullptr);
	virtual ~CFarmland();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// 조립 객체를 따로 따로 렌더링 하려는 경우.
	// 혜택. y값 정렬 렌더링
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	
public:
	void GenerateCrops(void);
	void GeneratePig(void);
	bool Cropped(float _fCroppedAmount);
	void DestroyFarmland(void);

private:
	CFarmland::E_STATE m_eState = STATE_UNOCCUPIED;
	CPig* m_pPig = nullptr;
	vector<CCrop*> m_vecCrops;
	float m_fCropOffsetY = -7.f;
	float m_fElapsedTime = 0.f;
};

