#pragma once
#include "CComDepObj.h"

class CFarmland;
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
	virtual void Render(CCamera* _pCamera);
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	//virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList) { _vecRenderList.emplace_back(this); }
	virtual void Release(void) = 0;

public:
	bool Cropping(float _fDeltaTime);

private:
	CFarmland* m_pFarmland = nullptr;
};

