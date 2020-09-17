#pragma once
#include "CScene.h"

class CSpriteObj;
class CCommander;
class CMapLoader;
class CUI_InGameUI;
class CUI_FadeInOut;
class CObserver;

class CPlayScene :
	public CScene
{
public:
	CPlayScene(CGameWorld& _rGameWorld);
	virtual ~CPlayScene();

	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

private:
	//CSpriteObj* m_pMap = nullptr;
	CCommander* m_pCommander[4] = { nullptr, };
	CUI_InGameUI* m_pInGameUI = nullptr;
	CObserver* m_pObserver = nullptr;
	bool m_bIsObserverMode = false;
	bool m_bIsTilesRenderring = true;

	CUI_FadeInOut* m_pFadeInOutUI = nullptr;
};

