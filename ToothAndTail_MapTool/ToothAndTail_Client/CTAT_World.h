#pragma once
#include "CGameWorld.h"
class CTAT_World :
	public CGameWorld
{
public:
	CTAT_World();
	virtual ~CTAT_World();

public:
	// CGameWorld��(��) ���� ��ӵ�
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void SetAnotherCameraTemporarily(CCamera* _pAnotherCamera, float _fKeepTime);

private:
	virtual void LoadResources();

private:
	// �̺�Ʈ
	bool m_bIsCameraEventOccurring = false;
	CCamera* m_pAnotherCamera = nullptr;
	float m_fCameraKeepTime = 0.f;
};

