#pragma once
#include "CObj.h"

class CCamera;
class CUI_Image;
class CUI_Minimap :
	public CObj
{
public:
	CUI_Minimap(CGameWorld& _rGameWorld, const D3DXVECTOR3& _vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	virtual ~CUI_Minimap();

	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(void);

public:
	CCamera* m_pCamera = nullptr;
	CUI_Image* m_pMinimapBase = nullptr;
	CUI_Image* m_pMinimapSign = nullptr;
};

