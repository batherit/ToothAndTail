#pragma once
#include "CSpriteObj.h"

class CCommander;
class CComDepObj :
	public CSpriteObj
{
public:
	CComDepObj(CGameWorld& _rGameWorld, CCommander* _pCommander, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f, int _iID = -1);
	virtual ~CComDepObj();

public:
	virtual void Render(CCamera* _pCamera) override;
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) override;
	virtual int UpdateAnim(float _fDeltaTime);
	virtual void Release(void) override;

public:
	void GenerateIdentificationTintObj(size_t _iWidth, size_t _iHeight, const wstring& _wstrTintKey, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DCOLOR GetIdentificationTint(void) {
		D3DCOLOR clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255);
		if (m_pIdentificationTintSprite)
			clIdentificationTint = m_pIdentificationTintSprite->GetRenderColor();
		return clIdentificationTint;
	}

	// 명령 관련
	int GetID(void) const { return m_iID; }
	virtual void SetCommander(CCommander* _pCommander, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	CCommander* GetCommander(void) const { return m_pCommander; }
	void SetTargetPos(const D3DXVECTOR3& _vTargetPos) { m_vTargetPos = _vTargetPos; }

	// 타겟 지점에 위치했는지? 이동 여부를 결정할 때 쓰인다.
	// 이 함수를 호출하기 이전에 유효한 목표지점을 세팅해두고 있어야 한다.
	bool IsLocatedAtTargetPoint(void) const;		

	// 이동할 때 쓰인다.
	bool GoToTargetPoint(float _fDeltaTime);		
	
	

protected:
	void SetID(int _iID) { m_iID = _iID; }
	void UpdateSpriteDir(void);

private:
	int m_iID = -1;
	CCommander* m_pCommander = nullptr;
	CSpriteObj* m_pIdentificationTintSprite = nullptr; // 팀 구분 색상 객체
	D3DXVECTOR3 m_vTargetPos;
};

