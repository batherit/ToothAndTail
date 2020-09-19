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
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual void InvalidateObj(void) override;
	virtual void TakeDamage(float _fDamageAmount, CObj* _pAttacker) override;
	D3DXVECTOR3 GetAttackerPos() const { return m_vAttackerPos; }

public:
	CStateMgr<CPig>* GetStateMgr(void) const { return m_pStateMgr; }
	void StartPatrol(void);
	int UpdatePatrol(float _fDeltaTime);
	bool IsAttackedRecently() const { return m_bIsAttackedRecently; }

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

	bool m_bIsAttackedRecently = false;
	float m_fAttackedTickTime = 0.f;

	D3DXVECTOR3 m_vAttackerPos;
};

