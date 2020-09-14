#pragma once
#include "CGameWorld.h"
class CTAT_World :
	public CGameWorld
{
public:
	CTAT_World();
	virtual ~CTAT_World();

	// CGameWorld��(��) ���� ��ӵ�
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	virtual void LoadResources();
};

