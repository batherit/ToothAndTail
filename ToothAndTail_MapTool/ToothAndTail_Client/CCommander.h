#pragma once
#include "CSpriteObj.h"

template<typename T> class CStateMgr;
class CCommander :
	public CSpriteObj
{
public:
	// 깃을 흔들고 있지 않은 상태, 부대 제어, 병력 제어
	enum E_FLAG_TYPE { FLAG_TYPE_NONE, FLAG_TYPE_UNIT, FLAG_TYPE_MILITARY };	

public:
	CCommander(CGameWorld& _rGameWorld, float _fX, float _fY, const wstring& _strComName = L"COM_COMMONER", D3DCOLOR _clIdentificationTint_ARGB = D3DCOLOR_ARGB(255, 255, 255, 255));
	virtual ~CCommander();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) override;
	virtual int UpdateAnim(float _fDeltaTime);

public:
	CStateMgr<CCommander>* GetStateMgr(void) const { return m_pStateMgr; }
	bool IsMoveKeyPressed(float & _fToX, float & _fToY);
	bool IsBuildKeyPressed(void) const;
	bool IsFlagKeyPressed(CCommander::E_FLAG_TYPE& _eFlagType) const;

private:
	CSpriteObj* m_pIdentificationTintSprite = nullptr;
	CStateMgr<CCommander>* m_pStateMgr = nullptr;
};

