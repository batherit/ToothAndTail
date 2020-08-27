#include "stdafx.h"
#include "CCommander.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CStateMgr.h"
#include "CComState_Idle.h"


CCommander::CCommander(CGameWorld & _rGameWorld, float _fX, float _fY, const wstring& _strComName, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, COMMANDER_WIDTH, COMMANDER_HEIGHT, 1.f, 0.f, COMMANDER_SPEED)
{
	wstring wstrTint = _strComName;
	wstrTint += L"_TINT";

	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(_strComName));
	SetScaleXY(3.f, 3.f);

	m_pIdentificationTintSprite = new CSpriteObj(_rGameWorld, 0.f, 0.f, COMMANDER_WIDTH, COMMANDER_HEIGHT);
	m_pIdentificationTintSprite->SetParent(this);
	m_pIdentificationTintSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(wstrTint));
	m_pIdentificationTintSprite->SetColor(_clIdentificationTint_ARGB);

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
	SafelyDeleteObj(m_pIdentificationTintSprite);
	SafelyDeleteObj(m_pStateMgr);
}

void CCommander::Render(CCamera * _pCamera)
{
	CSpriteObj::RenderShadow(_pCamera);
	CSpriteObj::Render(_pCamera);
	m_pIdentificationTintSprite->Render(_pCamera);
}

void CCommander::SetNewAnimInfo(const AnimInfo & _stAnimInfo)
{
	CSpriteObj::SetNewAnimInfo(_stAnimInfo);
	m_pIdentificationTintSprite->SetNewAnimInfo(_stAnimInfo);
}

int CCommander::UpdateAnim(float _fDeltaTime)
{
	CSpriteObj::UpdateAnim(_fDeltaTime);
	return m_pIdentificationTintSprite->UpdateAnim(_fDeltaTime);
}

bool CCommander::IsMoveKeyPressed(float & _fToX, float & _fToY)
{
	_fToX = 0.f;
	_fToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_W) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		_fToX += cfDeltaX[OBJ::DIR_UP];
		_fToY += cfDeltaY[OBJ::DIR_UP];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_S) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		_fToX += cfDeltaX[OBJ::DIR_DOWN];
		_fToY += cfDeltaY[OBJ::DIR_DOWN];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_A) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		_fToX += cfDeltaX[OBJ::DIR_LEFT];
		_fToY += cfDeltaY[OBJ::DIR_LEFT];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_D) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		_fToX += cfDeltaX[OBJ::DIR_RIGHT];
		_fToY += cfDeltaY[OBJ::DIR_RIGHT];
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
