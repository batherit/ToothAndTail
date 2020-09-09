#include "stdafx.h"
#include "CUI_InGameUI.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"
#include "CUI_Minimap.h"
#include "CUI_UnitSign.h"
#include "CCommander.h"
#include "CTunnelGenerator.h"


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

	// 유닛 표식
	if (_pCommander) {
		CUI_UnitSign* pUnitSign = nullptr;
		vector<CTunnelGenerator*>& rTunnelGenerators = _pCommander->GetTunnelGenerators();
		int iTunnelGeneratorSize = rTunnelGenerators.size();
		float fUnitSignGap = static_cast<float>(m_pMiddleWoodBack->GetWidth()) / iTunnelGeneratorSize;
		float fStartX = m_pMiddleWoodBack->GetOutputArea().left;
		for (int i = 0; i < iTunnelGeneratorSize; ++i) {
			pUnitSign = new CUI_UnitSign(_rGameWorld, _pCommander, rTunnelGenerators[i]->GetUnitType(),
				D3DXVECTOR3(fStartX + fUnitSignGap * (0.5f + i), WINCY - 89.f * 0.7f * 0.5f + 1 - 20, 0.f));
			m_vecUnitSigns.emplace_back(pUnitSign);
		}
		m_pUnitSignArrow = new CUI_Image(_rGameWorld, 
			CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
		// pogarrows = 806 1438 166 166
		m_pUnitSignArrow->SetExtractionArea(806, 1438, 806 + 166, 1438 + 166);
		m_pUnitSignArrow->SetOutputArea(POINT({ -1, -1 }), 100, 100);
	}
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
	// UI 백그라운드
	SafelyDeleteObj(m_pLeftWoodBack);
	SafelyDeleteObj(m_pMiddleWoodBack);
	SafelyDeleteObj(m_pRightWoodBack);
	// 미니맵
	SafelyDeleteObj(m_pMinimap);
	// 유닛 표식
	SafelyDeleteObjs(m_vecUnitSigns);
	SafelyDeleteObj(m_pUnitSignArrow);
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

	if (m_pCommander) {
		for (int i = 0; i < m_vecUnitSigns.size(); ++i) {
			m_vecUnitSigns[i]->SetScale(1.f);
			// 현재 선택된 유닛 표식은 크게 키워준다.
			if (m_pCommander->GetTunnelGeneratorIndex() == i) {
				m_vecUnitSigns[i]->SetScale(1.2f);
				// 현재 선택된 유닛 주변에 화살표 표시를 렌더해준다.
				m_pUnitSignArrow->SetXY(m_vecUnitSigns[i]->GetXY());
				m_pUnitSignArrow->Render(nullptr);
			}
			m_vecUnitSigns[i]->Render(nullptr);
		}
	}
}
