#include "stdafx.h"
#include "CUI_InGameUI.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"


CUI_InGameUI::CUI_InGameUI(CGameWorld & _rGameWorld, CCommander * _pCommander, const D3DXVECTOR3 & _rPos)
	:
	CObj(_rGameWorld, _rPos.x, _rPos.y),
	m_pCommander(_pCommander)
{
	m_pLeftWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pLeftWoodBack->SetParent(this);
	//0 424 613 188
	m_pLeftWoodBack->SetExtractionArea(0, 424, 0 + 613, 424 + 188);
	POINT ptPos;
	ptPos.x = 613.f * 0.7f * 0.5f;
	ptPos.y = WINCY - 188.f * 0.7f * 0.5f;
	m_pLeftWoodBack->SetOutputArea(ptPos, 613.f * 0.7f, 188.f * 0.7f);

	m_pMiddleWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pMiddleWoodBack->SetParent(this);
	//0 334 727 89
	m_pMiddleWoodBack->SetExtractionArea(0, 334, 727, 334 + 89);
	ptPos.x = WINCX >> 1;
	ptPos.y = WINCY - 89.f * 0.7f * 0.5f;
	m_pMiddleWoodBack->SetOutputArea(ptPos, 727.f * 0.7f, 89.f * 0.7f);

	m_pRightWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pRightWoodBack->SetParent(this);
	//614 424 405 137
	m_pRightWoodBack->SetExtractionArea(614, 424, 614 + 405, 424 + 137);
	ptPos.x = WINCX - 405.f * 0.7f * 0.5f;
	ptPos.y = WINCY - 137.f * 0.7f * 0.5f;
	m_pRightWoodBack->SetOutputArea(ptPos, 405.f * 0.7f, 137.f * 0.7f);
}

CUI_InGameUI::~CUI_InGameUI()
{
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
}

void CUI_InGameUI::Render(CCamera * _pCamera)
{
	m_pLeftWoodBack->Render(nullptr);
	m_pMiddleWoodBack->Render(nullptr);
	m_pRightWoodBack->Render(nullptr);
}
