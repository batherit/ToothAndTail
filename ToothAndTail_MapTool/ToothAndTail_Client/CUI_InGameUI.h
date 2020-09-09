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

	// CObj��(��) ���� ��ӵ�
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

private:
	CCommander* m_pCommander = nullptr;	// � ����� ���¸� ��Ÿ�� ���ΰ�?

	// ��׶��� UI
	CUI_Image* m_pLeftWoodBack = nullptr;
	CUI_Image* m_pMiddleWoodBack = nullptr;
	CUI_Image* m_pRightWoodBack = nullptr;
	// �̴ϸ� UI
	CUI_Minimap* m_pMinimap = nullptr;
	// ���� ǥ��
	vector<CUI_UnitSign*> m_vecUnitSigns;
	CUI_Image* m_pUnitSignArrow = nullptr;
};

