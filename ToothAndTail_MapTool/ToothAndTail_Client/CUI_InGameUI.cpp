#include "stdafx.h"
#include "CUI_InGameUI.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"
#include "CUI_Minimap.h"
#include "CCommander.h"


CUI_InGameUI::CUI_InGameUI(CGameWorld & _rGameWorld, CCommander * _pCommander, const D3DXVECTOR3 & _rPos)
	:
	CObj(_rGameWorld, _rPos.x, _rPos.y),
	m_pCommander(_pCommander)
{
	// 인게임 백그라운드 UI
	m_pLeftWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pLeftWoodBack->SetParent(this);
	//0 424 613 188
	m_pLeftWoodBack->SetExtractionArea(0, 424, 0 + 613, 424 + 188);
	POINT ptPos;
	ptPos.x = 613.f * 0.7f * 0.5f;
	ptPos.y = WINCY - 188.f * 0.7f * 0.5f + 1;
	m_pLeftWoodBack->SetOutputArea(ptPos, 613.f * 0.7f, 188.f * 0.7f);

	m_pMiddleWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pMiddleWoodBack->SetParent(this);
	//0 334 727 89
	m_pMiddleWoodBack->SetExtractionArea(0, 334, 727, 334 + 89);
	ptPos.x = (WINCX >> 1) + 80.f;
	ptPos.y = WINCY - 89.f * 0.7f * 0.5f + 1;
	m_pMiddleWoodBack->SetOutputArea(ptPos, 727.f * 0.7f, 89.f * 0.7f);

	m_pRightWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pRightWoodBack->SetParent(this);
	//614 424 405 137
	m_pRightWoodBack->SetExtractionArea(614, 424, 614 + 405, 424 + 137);
	ptPos.x = WINCX - 405.f * 0.7f * 0.5f + 1;
	ptPos.y = WINCY - 137.f * 0.7f * 0.5f + 1;
	m_pRightWoodBack->SetOutputArea(ptPos, 405.f * 0.7f, 137.f * 0.7f);

	
	// 미니맵
	m_pMinimap = new CUI_Minimap(_rGameWorld, D3DXVECTOR3(256 >> 1, WINCY -120.f, 0.f));
	
}

CUI_InGameUI::~CUI_InGameUI()
{
	Release();
}

void CUI_InGameUI::Ready(void)
{
}

int CUI_InGameUI::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_InGameUI::LateUpdate(void)
{
}

void CUI_InGameUI::Release(void)
{
	SafelyDeleteObj(m_pLeftWoodBack);
	SafelyDeleteObj(m_pMiddleWoodBack);
	SafelyDeleteObj(m_pRightWoodBack);

	SafelyDeleteObj(m_pMinimap);
}

void CUI_InGameUI::Render(CCamera * _pCamera)
{
	m_pLeftWoodBack->Render(nullptr);
	
	TCHAR szBuf[64] = L"NULL";
	if (m_pCommander) {
		swprintf_s(szBuf, L"%d", m_pCommander->GetMoney());	
	}
	CGraphicDevice::GetInstance()->RenderText(szBuf, D3DXVECTOR3(260.f, WINCY - 70.f, 0.f));

	m_pMiddleWoodBack->Render(nullptr);
	m_pRightWoodBack->Render(nullptr);

	m_pMinimap->Render();
}
