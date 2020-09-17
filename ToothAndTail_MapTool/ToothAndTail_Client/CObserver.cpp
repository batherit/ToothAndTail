#include "stdafx.h"
#include "CObserver.h"
#include "CCamera.h"

CObserver::CObserver(CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, WINCX, WINCY)
{
	m_pPrivateCamera = new CCamera(_rGameWorld, this, 0.f, 0.f);
}

CObserver::~CObserver()
{
	Release();
}

void CObserver::Ready(void)
{
}

int CObserver::Update(float _fDeltaTime)
{
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		// ╩С
		vDir.y -= 1.f;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		// го
		vDir.y += 1.f;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		// аб
		vDir.x -= 1.f;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		// ©Л
		vDir.x += 1.f;
	}

	if (D3DXVec3Length(&vDir) <= 0.00001f) {
		SetSpeed(0.f);
		return 1;
	}

	D3DXVec3Normalize(&vDir, &vDir);
	SetToXY(vDir);
	SetSpeed(m_pPrivateCamera->GetZoomMultiple() * OBSERVER_SPEED);
	MoveByDeltaTime(_fDeltaTime);

	return 0;
}

void CObserver::LateUpdate(void)
{
}

void CObserver::Release(void)
{
	SafelyDeleteObj(m_pPrivateCamera);
}

void CObserver::ZoomIn(float _fDeltaZoom)
{
	m_pPrivateCamera->ZoomIn(_fDeltaZoom);
}

void CObserver::ZoomOut(float _fDeltaZoom)
{
	m_pPrivateCamera->ZoomOut(_fDeltaZoom);
}

void CObserver::SetZoomMultiple(float _fZoomMultiple)
{
	m_pPrivateCamera->SetZoomMultiple(_fZoomMultiple);
}

float CObserver::GetZoomMultiple() const
{
	return m_pPrivateCamera->GetZoomMultiple();
}
