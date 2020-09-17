#pragma once
#include "CObj.h"

class CCamera;
class CObserver :
	public CObj
{
public:
	CObserver(CGameWorld& _rGameWorld);
	virtual ~CObserver();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

public:
	CCamera* GetObserverCamera() const { return m_pPrivateCamera; }
	void ZoomIn(float _fDeltaZoom);
	void ZoomOut(float _fDeltaZoom);
	void SetZoomMultiple(float _fZoomMultiple);
	float GetZoomMultiple() const;

private:
	CCamera* m_pPrivateCamera = nullptr;
};

