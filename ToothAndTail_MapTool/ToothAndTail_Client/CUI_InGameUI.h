#pragma once
#include "CObj.h"

class CCommander;
class CUI_Image;
class CCamera;
class CUI_Minimap;
class CUI_UnitSign;

class CUI_InGameUI :
	public CObj
{
public:
	CUI_InGameUI(CGameWorld& _rGameWorld, CCommander* _pCommander, const D3DXVECTOR3& _rPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	virtual ~CUI_InGameUI();

	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

private:
	CCommander* m_pCommander = nullptr;	// 어떤 기수의 상태를 나타낼 것인가?

	// 백그라운드 UI
	CUI_Image* m_pLeftWoodBack = nullptr;
	CUI_Image* m_pMiddleWoodBack = nullptr;
	CUI_Image* m_pRightWoodBack = nullptr;
	// 미니맵 UI
	CUI_Minimap* m_pMinimap = nullptr;
	// 유닛 표식
	vector<CUI_UnitSign*> m_vecUnitSigns;
	CUI_Image* m_pUnitSignArrow = nullptr;
};

