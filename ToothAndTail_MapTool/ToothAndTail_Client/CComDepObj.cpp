#include "stdafx.h"
#include "CComDepObj.h"
#include "CCommander.h"
#include "CTextureMgr.h"



CComDepObj::CComDepObj(CGameWorld & _rGameWorld, CCommander* _pCommander, float _fX, float _fY, size_t _iWidth, size_t _iHeight, float _fToX, float _fToY, float _fSpeed)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fToX, _fToY, _fSpeed),
	m_pCommander(_pCommander)
{
}

CComDepObj::~CComDepObj()
{
	Release();
}

void CComDepObj::Render(CCamera * _pCamera)
{
	CSpriteObj::Render(_pCamera);
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->Render(_pCamera);
}

void CComDepObj::SetNewAnimInfo(const AnimInfo & _stAnimInfo)
{
	CSpriteObj::SetNewAnimInfo(_stAnimInfo);
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->SetNewAnimInfo(_stAnimInfo);
}

int CComDepObj::UpdateAnim(float _fDeltaTime)
{
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->UpdateAnim(_fDeltaTime);
	return CSpriteObj::UpdateAnim(_fDeltaTime);
}

void CComDepObj::Release(void)
{
	SafelyDeleteObj(m_pIdentificationTintSprite);
}

void CComDepObj::GenerateIdentificationTintObj(size_t _iWidth, size_t _iHeight, const wstring & _wstrTintKey, D3DXCOLOR _clIdentificationTint)
{
	if (m_pIdentificationTintSprite) SafelyDeleteObj(m_pIdentificationTintSprite);
	// 색 정보를 나타내는 틴트 텍스처 장착
	m_pIdentificationTintSprite = new CSpriteObj(GetGameWorld(), 0.f, 0.f, _iWidth, _iHeight);
	m_pIdentificationTintSprite->SetParent(this);
	m_pIdentificationTintSprite->SetRenderOffsetXY(GetRenderOffsetXY());
	if(m_pCommander)
		m_pIdentificationTintSprite->SetColor(m_pCommander->GetIdentificationTint());
	else
		m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
	m_pIdentificationTintSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(_wstrTintKey));
}

void CComDepObj::SetCommander(CCommander * _pCommander, D3DXCOLOR _clIdentificationTint){
	m_pCommander = _pCommander;
	if (m_pCommander) {
		if (m_pIdentificationTintSprite)
			if (m_pCommander)
				m_pIdentificationTintSprite->SetColor(_pCommander->GetIdentificationTint());
			else
				m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
		return;
	}
	if (m_pIdentificationTintSprite) m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
}
