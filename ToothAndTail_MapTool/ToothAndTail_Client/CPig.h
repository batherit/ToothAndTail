#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CFarmland;
class CCamera;
class CPig :
	public CComDepObj
{
public:
	CPig(CGameWorld& _rGameWorld, CFarmland* _pFarmland, CCommander* _pCommander = nullptr);
	virtual ~CPig();

public:
	virtual void Ready(void);
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	// 조립 객체를 따로 따로 렌더링 하려는 경우.
	// 혜택. y값 정렬 렌더링
	//virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList) { _vecRenderList.emplace_back(this); }
	virtual void Release(void);
	virtual void Render(CCamera* _pCamera) override;

public:
	CStateMgr<CPig>* GetStateMgr(void) const { return m_pStateMgr; }
	bool Cropping(float _fDeltaTime);
	void StartPatrol(void);
	int UpdatePatrol(float _fDeltaTime);

private:
	const D3DXVECTOR3 m_cvLocationsToPatrol[4] = {
		D3DXVECTOR3(0.f, -35.f, 0.f),
		D3DXVECTOR3(20.f, -16.f, 0.f),
		D3DXVECTOR3(0.f, 5.f, 0.f),
		D3DXVECTOR3(-20.f, -16.f, 0.f)
	};
	int m_iLocationIndex = 0;
	CStateMgr<CPig>* m_pStateMgr = nullptr;
	CFarmland* m_pFarmland = nullptr;
};

