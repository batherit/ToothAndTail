#pragma once
#include "CGameWorld.h"
class CSpriteObj;
class CMapEditor :
	public CGameWorld
{
public:
	CMapEditor();
	virtual ~CMapEditor();

	// CGameWorld을(를) 통해 상속됨
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	void LoadTextures(void);

private:
	CCamera* m_pCamera = nullptr;
	CSpriteObj* m_pMap = nullptr;
};

