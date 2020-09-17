#pragma once
#include "CScene.h"

template<typename T> class CUI_Button;
class CUI_Image;

class CTitleScene :
	public CScene
{
public:
	CTitleScene(CGameWorld& _rGameWorld);
	virtual ~CTitleScene();

	// CScene을(를) 통해 상속됨
	//virtual void Ready(void) override;
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Render(CCamera* _pCamera) override;
	virtual void Release(void) override;
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	// button event
	void HoveredOnStartButton(void*);
	void UnhoveredOnStartButton(void*);
	void ClickStartButton(void*);
	void HoveredOnExitButton(void*);
	void UnhoveredOnExitButton(void*);
	void ClickQuitButton(void*);

private:
	CUI_Image* m_pTitleImage = nullptr;

	bool m_bIsOnStartButtonHovered = false;
	CUI_Image* m_pStartButtonOn = nullptr;
	CUI_Image* m_pStartButtonOff = nullptr;
	CUI_Image* m_pStartButtonImage = nullptr;
	CUI_Button<CTitleScene>* m_pStartButton = nullptr;

	bool m_bIsOnExitButtonHovered = false;
	CUI_Image* m_pExitButtonOn = nullptr;
	CUI_Image* m_pExitButtonOff = nullptr;
	CUI_Image* m_pExitButtonImage = nullptr;
	CUI_Button<CTitleScene>* m_pExitButton = nullptr;
};

