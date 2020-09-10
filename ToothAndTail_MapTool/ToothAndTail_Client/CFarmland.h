#pragma once
#include "CComDepObj.h"

class CCommander;
class CPig;
class CCrop;
class CUI_BuildGauge;
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
	// ���� ��ü�� ���� ���� ������ �Ϸ��� ���.
	// ����. y�� ���� ������
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	virtual void Release(void);
	virtual void InvalidateObj() override;
	//virtual void CollectGarbageObjs();
	
public:
	CFarmland::E_STATE GetFarmlandState() const { return m_eState; }
	void GenerateCrops(void);
	void GeneratePig(void);
	int Cropped(void);	// ��Ȯ�� ���� ��ȯ�ȴ�. ��, ��Ȯ�� �� ������ 0�� ��ȯ�Ѵ�.
	void ReleasePig(CPig* _pPig);
	virtual void SetCommander(CCommander* _pCommander, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	//void Occupied(CCommander* _pCommander);

private:
	CFarmland::E_STATE m_eState = STATE_UNOCCUPIED;
	int m_iCurrentFertility = 0;

	CPig* m_pPig = nullptr;
	vector<CCrop*> m_vecCrops;
	float m_fCropOffsetY = -7.f;
	float m_fElapsedTime = 0.f;

	bool m_bIsCropGenerated = false;
	bool m_bIsPigGenerating = false;
	CUI_BuildGauge* m_pBuildGauge = nullptr;
};

