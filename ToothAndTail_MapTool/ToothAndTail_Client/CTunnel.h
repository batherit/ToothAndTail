#pragma once
#include "CComDepObj.h"

//class CUnitGenerator;
class CCommander;
class CUI_BuildGauge;
class CTunnelGenerator;
class CTunnel final:
	public CComDepObj 
{
public:
	enum E_SIZE { SIZE_SMALL, SIZE_MIDDLE, SIZE_BIG, SIZE_END };

public:
	//CTunnel(CGameWorld& _rGameWorld, float _fX, float _fY, CTunnel::E_SIZE _eSize, UNIT::E_TYPE _eUnitType, CCommander* _pCommander = nullptr, int _iID = -1);
	CTunnel(CGameWorld& _rGameWorld, CTunnelGenerator* _pTunnelGenerator, const TileSiteInfo& _rTileSiteInfo, CTunnel::E_SIZE _eSize, UNIT::E_TYPE _eUnitType, CCommander* _pCommander = nullptr, int _iID = -1);
	virtual ~CTunnel();

	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Release(void) override;
	virtual void InvalidateObj(void) override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList);
	
public:
	// 소속된 유닛이 호출한다.
	CTunnelGenerator* GetTunnelGenerator() const { return m_pTunnelGenerator; }
	void ReleaseUnit(CComDepObj* _pUnit) { 
		m_listUnits.remove(_pUnit);
	} 
	int GetMaxSupplyNum() const { return m_iMaxSupplyNum; }

private:
	enum E_STATE { STATE_BUILDING, STATE_COMPLETED, STATE_GENERATE_UNIT, STATE_END };

private:
	int m_iID = -1;
	//int m_iSupplyNum = 0;
	CTunnelGenerator* m_pTunnelGenerator = nullptr;

	list<CComDepObj*> m_listUnits;	// 생성한 유닛들 (땅굴이 파괴되면 유닛 소속 터널을 제거해주기 위함.)
	UNIT::E_TYPE m_eUnitType = UNIT::TYPE_END;
	CTunnel::E_SIZE m_eSize = CTunnel::SIZE_SMALL;
	CTunnel::E_STATE m_eState = CTunnel::STATE_BUILDING;

	float m_fGenTime = 0.f;		// 젠 시간 : ElapsedTime이 이를 초과하면 유닛을 생성함.
	int m_iMaxSupplyNum = 0;	// 최대 보급 수 : 이 수를 넘어서 생성할 수 없다.
	int m_iUnitCost = 0;		// 생성할 유닛 비용

	CUI_BuildGauge* m_pBuildGauge = nullptr;

	bool m_bIsGenerating = false;
	float m_fElapsedTime = 0.f;
};

