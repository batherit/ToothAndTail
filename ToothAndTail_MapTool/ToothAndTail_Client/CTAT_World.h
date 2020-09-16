#pragma once
#include "CGameWorld.h"
class CTAT_World :
	public CGameWorld
{
public:
	CTAT_World();
	virtual ~CTAT_World();

public:
	// CGameWorld을(를) 통해 상속됨
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void SetAnotherCameraTemporarily(CCamera* _pAnotherCamera, float _fKeepTime);
	virtual void JudgetResult() override;

private:
	enum E_RESULT { RESULT_WIN, RESULT_LOSE, RESULT_YET, RESULT_END };

private:
	virtual void LoadResources();

private:
	// 이벤트
	bool m_bIsCameraEventOccurring = false;
	CCamera* m_pAnotherCamera = nullptr;
	float m_fCameraKeepTime = 0.f;
	CTAT_World::E_RESULT m_eGameResult = CTAT_World::RESULT_YET;
};

