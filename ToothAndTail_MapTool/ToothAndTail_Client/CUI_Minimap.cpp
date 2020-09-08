#include "stdafx.h"
#include "CUI_Minimap.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"
#include "CCamera.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CComDepObj.h"


CUI_Minimap::CUI_Minimap(CGameWorld & _rGameWorld, const D3DXVECTOR3& _vPos)
	:
	CObj(_rGameWorld, _vPos.x, _vPos.y)
{
	m_pMinimapBase = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"MINIMAP"));
	m_pMinimapBase->SetParent(this);
	m_pCamera = new CCamera(_rGameWorld);

	CUI_Image* pMapImage = _rGameWorld.GetMapLoader()->GetMapImage();
	float fRatio = pMapImage->GetWidth() / static_cast<float>(m_pMinimapBase->GetWidth());
	m_pCamera->SetXY(
		(pMapImage->GetWidth() >> 1) + fRatio * ((WINCX >> 1) - _vPos.x), 
		(pMapImage->GetHeight() >> 1) + fRatio * ((WINCY >> 1) - _vPos.y));
	m_pCamera->SetZoomMultiple(1.f/fRatio);

	m_pMinimapSign = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"MINIMAP_SIGN"));

}

CUI_Minimap::~CUI_Minimap()
{
	Release();
}

void CUI_Minimap::Ready(void)
{
}

int CUI_Minimap::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_Minimap::LateUpdate(void)
{
}

void CUI_Minimap::Release(void)
{
	SafelyDeleteObj(m_pCamera);
	SafelyDeleteObj(m_pMinimapBase);
	SafelyDeleteObj(m_pMinimapSign);
}

void CUI_Minimap::Render()
{
	m_pMinimapBase->Render(nullptr);

	CComDepObj* pMinimapObj = nullptr;
	D3DXVECTOR3 vPos;
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pMinimapObj = dynamic_cast<CComDepObj*>(pObj);
		if (!pMinimapObj) continue;

		m_pMinimapSign->SetRenderColor(pMinimapObj->GetIdentificationTint());
		m_pMinimapSign->SetXY(pMinimapObj->GetXY());
		switch (pMinimapObj->GetMinimapSign())
		{
		case MINIMAP::SIGN_COMMANDER:
			m_pMinimapSign->SetScale(2.0f);
			m_pMinimapSign->Render(m_pCamera);
			break;
		case MINIMAP::SIGN_UNIT:
			m_pMinimapSign->SetScale(1.0f);
			m_pMinimapSign->Render(m_pCamera);
			break;
		case MINIMAP::SIGN_TUNNEL:
			m_pMinimapSign->SetScale(1.5f);
			m_pMinimapSign->Render(m_pCamera);
			break;
		case MINIMAP::SIGN_WINDMILL:
			m_pMinimapSign->SetScale(4.f);
			m_pMinimapSign->Render(m_pCamera);
			break;
		}
	}
}
