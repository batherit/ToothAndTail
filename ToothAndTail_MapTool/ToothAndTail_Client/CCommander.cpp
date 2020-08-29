#include "stdafx.h"
#include "CCommander.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CStateMgr.h"
#include "CComState_Idle.h"
#include "CComDepObj.h"



CCommander::CCommander(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, COMMANDER_WIDTH, COMMANDER_HEIGHT, 1.f, 0.f, COMMANDER_SPEED),
	m_eCommanderType(_eCommanderType)/*,
	m_clIdentificationTint_ARGB(_clIdentificationTint_ARGB)*/
{
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	wstring wstrCommander = L"";
	switch (_eCommanderType)
	{
	case CCommander::COM_TYPE_COMMONER:
		wstrCommander = L"COM_COMMONER";
		break;
	case CCommander::COM_TYPE_CAPITALIST:
		wstrCommander = L"COM_CAPITALIST";
		break;
	case CCommander::COM_TYPE_MILITARY:
		wstrCommander = L"COM_MILITARY";
		break;
	case CCommander::COM_TYPE_CLERGY:
		wstrCommander = L"COM_CLERGY";
		break;
	default:
		break;
	}
	m_pCommanderSprite = new CComDepObj(_rGameWorld, nullptr, 0.f, -33.f, COMMANDER_WIDTH, COMMANDER_HEIGHT);
	m_pCommanderSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(wstrCommander));
	m_pCommanderSprite->SetShadow(true);
	m_pCommanderSprite->SetParent(this);
	m_pCommanderSprite->SetRenderLayer(10);
	m_pCommanderSprite->GenerateIdentificationTintObj(COMMANDER_WIDTH, COMMANDER_HEIGHT, wstrCommander + L"_TINT", _clIdentificationTint_ARGB);

	m_pStateMgr = new CStateMgr<CCommander>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CComState_Idle(GetGameWorld(), *this));
}

CCommander::~CCommander()
{
	Release();
}

void CCommander::Ready(void)
{
}

int CCommander::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CCommander::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CCommander::Release(void)
{
	SafelyDeleteObj(m_pCommanderSprite);
	//SafelyDeleteObj(m_pIdentificationTintSprite);
	SafelyDeleteObj(m_pStateMgr);
}

void CCommander::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	if (m_pCommanderSprite) m_pCommanderSprite->RegisterToRenderList(_vecRenderList);
	//if (m_pIdentificationTintSprite) m_pCommanderSprite->RegisterToRenderList(_vecRenderList);
}

void CCommander::SetNewAnimInfo(const AnimInfo & _stAnimInfo)
{
	return m_pCommanderSprite->SetNewAnimInfo(_stAnimInfo);
}

int CCommander::UpdateAnim(float _fDeltaTime)
{
	return m_pCommanderSprite->UpdateAnim(_fDeltaTime);
}

D3DCOLOR CCommander::GetIdentificationTint(void) const
{
	return m_pCommanderSprite->GetIdentificationTint();
}

bool CCommander::IsMoveKeyPressed(float & _fToX, float & _fToY)
{
	_fToX = 0.f;
	_fToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_W) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		_fToX += cfDeltaX[OBJ::DIR_UP];
		_fToY += cfDeltaY[OBJ::DIR_UP];
		SetScaleX(BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_S) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		_fToX += cfDeltaX[OBJ::DIR_DOWN];
		_fToY += cfDeltaY[OBJ::DIR_DOWN];
		SetScaleX(-BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_A) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		_fToX += cfDeltaX[OBJ::DIR_LEFT];
		_fToY += cfDeltaY[OBJ::DIR_LEFT];
		SetScaleX(-BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_D) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		_fToX += cfDeltaX[OBJ::DIR_RIGHT];
		_fToY += cfDeltaY[OBJ::DIR_RIGHT];
		SetScaleX(BASE_SCALE);
	}

	if (_fToX == 0.f && _fToY == 0.f) return false;
	return true;
}

bool CCommander::IsBuildKeyPressed(void) const
{
	return CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE);
}

bool CCommander::IsFlagKeyPressed(CCommander::E_FLAG_TYPE & _eFlagType) const
{
	_eFlagType = CCommander::FLAG_TYPE_NONE;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON) || CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_LBUTTON))
		_eFlagType = CCommander::FLAG_TYPE_UNIT;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON) || CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_RBUTTON))
		_eFlagType = CCommander::FLAG_TYPE_MILITARY;

	return _eFlagType != CCommander::FLAG_TYPE_NONE;
}