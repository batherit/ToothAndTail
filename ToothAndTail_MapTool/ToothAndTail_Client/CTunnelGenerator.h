#pragma once

class CGameWorld;
class CCommander;
class CTunnelGenerator final
{
public:
	CTunnelGenerator(CGameWorld& _rGameWorld, UNIT::E_TYPE _eUnitType, CCommander* _pCommander);
	virtual ~CTunnelGenerator();

public:
	bool GenerateTunnel();
	UNIT::E_TYPE GetUnitType() { return m_eUnitType; }

protected:
	CGameWorld& m_rGameWorld;
	UNIT::E_TYPE m_eUnitType = UNIT::TYPE_END;
	CCommander* m_pCommander = nullptr;
};

