#include "stdafx.h"
#include "CUI_UnitSign.h"
#include "CUI_Image.h"
#include "CCommander.h"
#include "CTextureMgr.h"

CUI_UnitSign::CUI_UnitSign(CGameWorld & _rGameWorld, CCommander* _pCommander, UNIT::E_TYPE _eUnitType, const D3DXVECTOR3 & vPos)
	:
	CObj(_rGameWorld, vPos.x, vPos.y)
{
	m_pUnitSignBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pUnitSignBack->SetParent(this);
	//  806 1605 130 130
	m_pUnitSignBack->SetExtractionArea(806, 1605, 806 + 130, 1605 + 130);
	m_pUnitSignBack->SetOutputArea(POINT({ 0, 0 }), 60, 60); // 본래 크기는 130*130이나 출력은 70*70으로 표시하도록 함.

	m_pUnitSign = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pUnitSign->SetParent(this);
	m_pUnitSignTint = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pUnitSignTint->SetParent(this);
	switch (_eUnitType) {
	case UNIT::TYPE_PIG:
		// unit_pig = 1454 1830 104 104
		// unit_pig_tint = 1559 1715 104 104
		m_pUnitSign->SetExtractionArea(1454, 1830, 1454 + 104, 1830 + 104);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1559, 1715, 1559 + 104, 1715 + 104);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	case UNIT::TYPE_SQUIRREL:
		// unit_squirrel = 1861 1533 104 104
		// unit_squirrel_tint = 1664 1757 104 104
		m_pUnitSign->SetExtractionArea(1861, 1533, 1861 + 104, 1533 + 104);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1664, 1757, 1664 + 104, 1757 + 104);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	case UNIT::TYPE_LIZARD:
		// unit_lizard = 1349, 1830 104 104
		// unit_lizard_tint = 1454, 1725 104 104
		m_pUnitSign->SetExtractionArea(1349, 1830, 1349 + 104, 1830 + 104);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1454, 1725, 1454 + 104, 1725 + 104);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	case UNIT::TYPE_MOLE:
		// unit_mole = 1651 1547 104 104
		// unit_mole_tint = 1756, 1428 104 104
		m_pUnitSign->SetExtractionArea(1651, 1547, 1651 + 104, 1547 + 104);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1756, 1428, 1756 + 104, 1428 + 104);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	case UNIT::TYPE_SKUNK:
		// unit_skunk = 1023 1759 114 106
		// unit_skunk_tint = 1576, 1220 114 106
		m_pUnitSign->SetExtractionArea(1023, 1759, 1023 + 104, 1759 + 104);
		m_pUnitSign->SetOutputArea(POINT({ -4, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1576, 1220, 1576 + 104, 1220 + 104);
		m_pUnitSignTint->SetOutputArea(POINT({ -4, -3 }), 46, 46);
		break;
	case UNIT::TYPE_BADGER:
		// unit_badger = 1720, 1089 105 114
		// unit_badger_tint = 1169, 1630 105 114
		m_pUnitSign->SetExtractionArea(1720, 1089, 1720 + 105, 1089 + 114);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1169, 1630, 1169 + 105, 1630 + 114);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	case UNIT::TYPE_FOX:
		// unit_fox = 1440, 1495 105 114
		// unit_fox_tint = 1826, 1103 105 114
		m_pUnitSign->SetExtractionArea(1440, 1495, 1440 + 105, 1495 + 114);
		m_pUnitSign->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		m_pUnitSignTint->SetExtractionArea(1826, 1103, 1826 + 105, 1103 + 114);
		m_pUnitSignTint->SetOutputArea(POINT({ 0, -3 }), 46, 46);
		break;
	}

	if (_pCommander) {
		m_pUnitSignTint->SetRenderColor(_pCommander->GetIdentificationTint());
	}
}

CUI_UnitSign::~CUI_UnitSign()
{
	Release();
}

void CUI_UnitSign::Ready(void)
{
}

int CUI_UnitSign::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_UnitSign::LateUpdate(void)
{
}

void CUI_UnitSign::Release(void)
{
	SafelyDeleteObj(m_pUnitSignBack);
	SafelyDeleteObj(m_pUnitSign);
	SafelyDeleteObj(m_pUnitSignTint);
}

void CUI_UnitSign::Render(CCamera * _pCamera)
{
	m_pUnitSignBack->Render(nullptr);
	m_pUnitSign->Render(nullptr);
	m_pUnitSignTint->Render(nullptr);
}
