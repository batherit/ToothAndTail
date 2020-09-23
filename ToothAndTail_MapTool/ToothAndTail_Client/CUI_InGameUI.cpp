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
	//m_pLeftWoodBack->SetExtractionArea(0, 424, 0 + 613, 424 + 188);
	// 515 742 411 92
	m_pLeftWoodBack->SetExtractionArea(515, 742, 515 + 411, 742 + 92);
	POINT ptPos;
	//ptPos.x = 613.f * 0.7f * 0.5f;
	// ptPos.y = WINCY - 188.f * 0.7f * 0.5f + 1;
	ptPos.x = static_cast<LONG>((411.f - 240.f)  * 1.1f * 0.5f);
	ptPos.y = static_cast<LONG>(92.f * 1.1f * 0.5f + 1.f);
	//m_pLeftWoodBack->SetOutputArea(ptPos, 613.f * 0.7f, 188.f * 0.7f);
	m_pLeftWoodBack->SetOutputArea(ptPos, static_cast<size_t>(411.f * 1.1f), static_cast<size_t>(92.f * 1.1f));
	//m_pLeftWoodBack->SetScale(1.7f);

	m_pMiddleWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pMiddleWoodBack->SetParent(this);
	//0 334 727 89
	m_pMiddleWoodBack->SetExtractionArea(0, 334, 727, 334 + 89);
	//ptPos.x = (WINCX >> 1) + 80.f;
	//ptPos.y = WINCY - 89.f * 0.7f * 0.5f + 1;
	ptPos.x = static_cast<LONG>(WINCX >> 1);
	ptPos.y = static_cast<LONG>(70.f * 0.7f * 0.5f + 1.f);
	m_pMiddleWoodBack->SetOutputArea(ptPos, static_cast<size_t>(727.f * 0.7f), static_cast<size_t>(89.f * 0.7f));
	m_pMiddleWoodBack->SetScaleY(-1.f);

	m_pRightWoodBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"), D3DXVECTOR3());
	m_pRightWoodBack->SetParent(this);
	//614 424 405 137
	m_pRightWoodBack->SetExtractionArea(614, 424, 614 + 405, 424 + 137);
	ptPos.x = static_cast<LONG>(WINCX - 405.f * 0.75f * 0.5f + 1.f);
	//ptPos.y = WINCY - 137.f * 0.7f * 0.5f + 1;
	ptPos.y = static_cast<LONG>(135.f * 0.7f * 0.5f + 1.f);
	m_pRightWoodBack->SetOutputArea(ptPos, static_cast<size_t>(405.f * 0.75f), static_cast<size_t>(137.f * 0.7f));
	m_pRightWoodBack->SetScaleY(-1.f);

	// 미니맵
	//m_pMinimap = new CUI_Minimap(_rGameWorld, D3DXVECTOR3(256 >> 1, WINCY -120.f, 0.f));
	m_pMinimap = new CUI_Minimap(_rGameWorld, D3DXVECTOR3(WINCX - (256 >> 1), 100.f, 0.f));

	// 유닛 표식
	if (_pCommander) {
		CUI_UnitSign* pUnitSign = nullptr;
		vector<CTunnelGenerator*>& rTunnelGenerators = _pCommander->GetTunnelGenerators();
		int iTunnelGeneratorSize = rTunnelGenerators.size();
		float fUnitSignGap = static_cast<float>(m_pMiddleWoodBack->GetWidth()) / iTunnelGeneratorSize;
		float fStartX = static_cast<float>(m_pMiddleWoodBack->GetOutputArea().left);
		for (int i = 0; i < iTunnelGeneratorSize; ++i) {
			pUnitSign = new CUI_UnitSign(_rGameWorld, _pCommander, rTunnelGenerators[i]->GetUnitType(),
				D3DXVECTOR3(fStartX + fUnitSignGap * (0.5f + i), /*WINCY - 89.f*/ 230.f * 0.7f * 0.5f + 1 - 20, 0.f));
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
	
	// 자본 표시
	TCHAR szBuf[64] = L"NULL";
	if (m_pCommander) {
		swprintf_s(szBuf, L"%d", m_pCommander->GetMoney());	
	}
	//CGraphicDevice::GetInstance()->RenderText(szBuf, D3DXVECTOR3(260.f, WINCY - 70.f, 0.f));
	CGraphicDevice::GetInstance()->RenderText(szBuf, D3DXVECTOR3(140.f,  5.f, 0.f));

	m_pMiddleWoodBack->Render(nullptr);
	m_pRightWoodBack->Render(nullptr);

	m_pMinimap->Render();

	swprintf_s(szBuf, L"%d/%d", 0, 0);
	D3DXVECTOR3 vPos;
	CTunnelGenerator* pTunnelGenerator = nullptr;
	if (m_pCommander) {
		for (int i = 0; i < static_cast<int>(m_vecUnitSigns.size()); ++i) {
			m_vecUnitSigns[i]->SetScale(1.f);
			// 현재 선택된 유닛 표식은 크게 키워준다.
			vPos = m_vecUnitSigns[i]->GetXY();
			if (m_pCommander->GetTunnelGeneratorIndex() == i) {
				m_vecUnitSigns[i]->SetScale(1.2f);
				// 현재 선택된 유닛 주변에 화살표 표시를 렌더해준다.
				m_pUnitSignArrow->SetXY(vPos);
				m_pUnitSignArrow->Render(nullptr);
			}
			// 유닛 표식 표시
			m_vecUnitSigns[i]->Render(nullptr);

			// 보급 수 표시
			vPos.x -= 20.f;
			//vPos.y += 20.f;
			vPos.y -= 60.f;
			pTunnelGenerator = m_pCommander->GetTunnelGenerators()[i];
			swprintf_s(szBuf, L"%d/%d", pTunnelGenerator->GetUnitsNum(), pTunnelGenerator->GetMaxSupplyNum());
			CGraphicDevice::GetInstance()->RenderText(szBuf, vPos, 0.35f);

		}
	}
}

void CUI_InGameUI::ChangeCommander(CCommander * _pCommander)
{
	if (!_pCommander) return;

	SafelyDeleteObjs(m_vecUnitSigns);

	CUI_UnitSign* pUnitSign = nullptr;
	vector<CTunnelGenerator*>& rTunnelGenerators = _pCommander->GetTunnelGenerators();
	int iTunnelGeneratorSize = rTunnelGenerators.size();
	float fUnitSignGap = static_cast<float>(m_pMiddleWoodBack->GetWidth()) / iTunnelGeneratorSize;
	float fStartX = static_cast<float>(m_pMiddleWoodBack->GetOutputArea().left);
	for (int i = 0; i < iTunnelGeneratorSize; ++i) {
		pUnitSign = new CUI_UnitSign(GetGameWorld(), _pCommander, rTunnelGenerators[i]->GetUnitType(),
			D3DXVECTOR3(fStartX + fUnitSignGap * (0.5f + i), /*WINCY - 89.f*/ 230.f * 0.7f * 0.5f + 1 - 20, 0.f));
		m_vecUnitSigns.emplace_back(pUnitSign);
	}

	m_pCommander = _pCommander;
}
