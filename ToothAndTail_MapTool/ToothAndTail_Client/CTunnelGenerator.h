#pragma once

class CGameWorld;
class CCommander;
class CTunnel;
class CTunnelGenerator final
{
public:
	CTunnelGenerator(CGameWorld& _rGameWorld, UNIT::E_TYPE _eUnitType, CCommander* _pCommander);
	virtual ~CTunnelGenerator();

public:
	bool GenerateTunnel(int _iID);
	UNIT::E_TYPE GetUnitType() const { return m_eUnitType; }
	void ReleaseTunnel(CTunnel* _pTunnel);
	void ReleaseUnit(CComDepObj* _pUnit);

public:
	int GetMaxSupplyNum() const { return m_iMaxSupplyNum; }
	int GetUnitsNum() const { return m_iUnitsNum; }
	int GetTunnelsNum() const { return m_iTunnelsNum; }

	void IncreaseMaxSupplyNum(int _iAmount) { 
		m_iMaxSupplyNum += _iAmount; 
		++m_iTunnelsNum;
	}
	void DecreaseMaxSupplyNum(int _iAmount) { 
		m_iMaxSupplyNum -= _iAmount;
		--m_iTunnelsNum;
	}
	void IncreaseUnitsNum() { ++m_iUnitsNum; }
	void DecreaseUnitsNum() { --m_iUnitsNum; }
	
private:
	CGameWorld& m_rGameWorld;
	UNIT::E_TYPE m_eUnitType = UNIT::TYPE_END;
	CCommander* m_pCommander = nullptr;

	int m_iMaxSupplyNum = 0;
	int m_iUnitsNum = 0;
	int m_iTunnelsNum = 0;
};

