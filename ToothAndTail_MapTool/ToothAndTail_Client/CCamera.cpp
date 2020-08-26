#include "stdafx.h"
#include "CCamera.h"


CCamera::CCamera(CGameWorld& _rGameWorld, CObj* _pOwner /*= nullptr*/, float _fX /*= 0.f*/, float _fY /*= 0.f*/)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, _fX, _fY, 0, 0, 0.f)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	else {
		SetSpeed(500.0f);
	}
}


CCamera::~CCamera()
{
	Release();
}

int CCamera::Update(float _fDeltaTime)
{
	// Èçµê ±â´É
	if (m_bIsShaking) {
		m_fShakeTickTime += _fDeltaTime;
		if (m_fShakeTickTime >= (m_fShakeKeepTime / m_iShakeNum)) {
			m_fShakeElapsedTime += m_fShakeTickTime;
			if (m_fShakeElapsedTime <= m_fShakeKeepTime) {
				m_fShakeTickTime = 0.f;
				m_fOffsetDegree = GetNumberMinBetweenMax(0.f, 360.f);
				float fT = 1.f - m_fShakeElapsedTime / m_fShakeKeepTime;
				m_fShakeOffsetX = cosf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
				m_fShakeOffsetY = sinf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
			}
			else {
				m_fShakeKeepTime = 0.f;
				m_fShakeOffsetX = 0.f;
				m_fShakeOffsetY = 0.f;
				m_bIsShaking = false;
			}
		}
	}

	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fShakeOffsetX);
		SetY(m_pOwner->GetY() + m_fShakeOffsetY);
	}
	return 0;
}

void CCamera::ZoomIn(float _fDeltaTime)
{
	Clamp(&(m_fZoomMultiple -= (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
}

void CCamera::ZoomOut(float _fDeltaTime)
{
	Clamp(&(m_fZoomMultiple += (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
}

void CCamera::Shake(float _fShakeKeepTime, float _fSakeRadius, int _iShakeNum)
{
	m_bIsShaking = true;
	m_fShakeKeepTime = _fShakeKeepTime;
	m_fShakeRadius = _fSakeRadius;
	m_iShakeNum = _iShakeNum;
	m_fShakeElapsedTime = 0.f;
	m_fShakeTickTime = 0.f;

	m_fOffsetDegree = GetNumberMinBetweenMax(0.f, 360.f);
	float fT = 1.f - m_fShakeElapsedTime / m_fShakeKeepTime;
	m_fShakeOffsetX = cosf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
	m_fShakeOffsetY = sinf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
}

RECT CCamera::GetScreenRect(RECT& _rRectW)
{
	RECT rc;

	rc.right	=	static_cast<LONG>((_rRectW.right - GetX()) * m_fZoomMultiple + (WINCX >> 1));
	rc.top		=	static_cast<LONG>((_rRectW.top - GetY())* m_fZoomMultiple + (WINCY >> 1));
	rc.left		=	static_cast<LONG>((_rRectW.left - GetX()) * m_fZoomMultiple + (WINCX >> 1));
	rc.bottom	=	static_cast<LONG>((_rRectW.bottom - GetY())* m_fZoomMultiple + (WINCY >> 1));

	return rc;
}

D3DXVECTOR3 CCamera::GetScreenPoint(const D3DXVECTOR3& _vPointW)
{
	D3DXVECTOR3 vPointS;
	vPointS.x = (_vPointW.x - GetX())* m_fZoomMultiple + (WINCX >> 1);
	vPointS.y = (_vPointW.y - GetY())* m_fZoomMultiple + (WINCY >> 1);
	vPointS.z = 0.f;

	return vPointS;
}

D3DXVECTOR3 CCamera::GetWorldPoint(const D3DXVECTOR3& _vPointS)
{
	D3DXVECTOR3 vPointW;
	vPointW.x = (_vPointS.x - (WINCX >> 1)) / m_fZoomMultiple + GetX();
	vPointW.y = (_vPointS.y - (WINCY >> 1)) / m_fZoomMultiple + GetY();
	vPointW.z = 0.f;

	return vPointW;
}

D3DXMATRIX CCamera::GetScreenMatrix(const D3DXMATRIX & _matWorld)
{
	D3DXMATRIX matScreen = _matWorld;
	D3DXVECTOR3 vPointS = GetScreenPoint(D3DXVECTOR3(_matWorld._41, _matWorld._42, _matWorld._43));
	
	matScreen._11 *= m_fZoomMultiple;
	matScreen._22 *= m_fZoomMultiple;
	matScreen._33 *= m_fZoomMultiple;
	matScreen._41 = vPointS.x;
	matScreen._42 = vPointS.y;
	matScreen._43 = vPointS.z;
	matScreen._44 = 1.f;

	return matScreen;
}

void CCamera::Release(void) {
	m_pOwner = nullptr;
}