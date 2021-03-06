#pragma once
#include "CComDepObj.h"

template<typename T> class CStateMgr;
class CComDepObj;
class CTunnelGenerator;
class CPathGanerator;
class CUI_BalloonTalk;

class CCommander :
	public CComDepObj
{
public:
	enum E_COM_TYPE { COM_TYPE_COMMONER, COM_TYPE_CAPITALIST, COM_TYPE_MILITARY, COM_TYPE_CLERGY, COM_TYPE_END };
	// 깃을 흔들고 있지 않은 상태, 부대 제어, 병력 제어
	//enum E_FLAG_TYPE { FLAG_TYPE_NONE, FLAG_TYPE_UNIT, FLAG_TYPE_MILITARY, FLAG_TYPE_END };	

public:
	CCommander(CGameWorld& _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType = COM_TYPE_END, D3DCOLOR _clIdentificationTint_ARGB = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual ~CCommander();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList) override; 
	//virtual void SetNewAnimInfo(const AnimInfo & _stAnimInfo) override;
	//virtual int UpdateAnim(float _fDeltaTime) override;
	virtual void MoveByDeltaTime(float _fDeltaTime);

public:
	CStateMgr<CCommander>* GetStateMgr(void) const { return m_pStateMgr; }
	//D3DCOLOR GetIdentificationTint(void) const;
	//CCommander::E_COM_TYPE GetCommanderType(void) const { return m_eCommanderType; }
	virtual bool IsMoving(float & _fToX, float & _fToY);		// AI측에서 재정의
	virtual bool IsActivating() const;							// AI측에서 재정의
	virtual bool IsSelling() const;
	virtual bool IsWavingFlag(/*CCommander::E_FLAG_TYPE& _eFlagType*/) const;// AI측에서 재정의

	UINT GetMoney() const { return m_iMoney; }
	void DecreseMoney(UINT _iAmount) { m_iMoney -= _iAmount; Clamp(&m_iMoney, MIN_MONEY, MAX_MONEY); }
	void IncreaseMoney(UINT _iAmount) { m_iMoney += _iAmount; Clamp(&m_iMoney, MIN_MONEY, MAX_MONEY); }
	void DesignateNextUnit() { 
		m_iTunnelGeneratorIndex = (m_iTunnelGeneratorIndex + 1) % m_vecTunnelGenerator.size();
	}
	void DesignatePrevUnit() { 
		m_iTunnelGeneratorIndex -= 1; 
		if (0 > m_iTunnelGeneratorIndex)
			m_iTunnelGeneratorIndex += m_vecTunnelGenerator.size();
	}
	int GetTunnelGeneratorIndex() const { return m_iTunnelGeneratorIndex; }
	bool GenerateTunnel();
	CommandInfo& GetCurrentCommandInfo() { return m_tCommandInfo; }
	void SetCommandInfo(const CommandInfo& _rCommandInfo) { m_tCommandInfo = _rCommandInfo; }
	vector<CTunnelGenerator*>& GetTunnelGenerators() { return m_vecTunnelGenerator; }
	int GetTotalUnitsNum() const;
	int GetTotalTunnelsNum() const;
	int GetSelectedUnitsNum() const;
	void ShoutOut(float _fOffsetX, float _fOffsetY, const wstring& _wstrText);
	int GetInstallableTunnelNum();
	vector<CWindmill*>& GetMyWindmills();

private:
	void UpdateCommand(float _fDeltaTime);

protected:
	bool DetectWindmills();
	
protected:
	CommandInfo m_tCommandInfo;
	CCommander::E_COM_TYPE m_eCommanderType = CCommander::COM_TYPE_END;
	UINT m_iMoney = 0;
	CUI_BalloonTalk* m_pBalloonTalkUI = nullptr;
	vector<CWindmill*> m_vecWindmills;
	vector<CWindmill*> m_vecMyWindmills;
	vector<CTunnelGenerator*> m_vecTunnelGenerator;
	int m_iTunnelGeneratorIndex = 0;

	float m_fElapsedTime = 0.f;

private:
	CStateMgr<CCommander>* m_pStateMgr = nullptr;
};

