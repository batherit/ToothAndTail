#pragma once

class CTimer;
class CSceneMgr;
class CSpace;
class CCamera;
class CObj;
class CGraphicDevice;

class CGameWorld abstract
{
public:
	CGameWorld();
	~CGameWorld();

public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Ready(void) = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(void) = 0;
	virtual void Release(void) = 0;
	CCamera* SetMainCamera(CCamera* _pCamera) { 
		CCamera* pOldCamera = m_pCamera;
		m_pCamera = _pCamera;
		return pOldCamera;
	}
	CCamera* GetMainCamera(void) const { return m_pCamera; }

public:
	bool ConfirmValidScene(void);
	void RunTick(void);
	void StartRender(void);
	void EndRender(void);
	CTimer* GetTimer(void) const { return m_pTimer; }
	CSceneMgr* GetSceneManager(void) const { return m_pSceneManager; }
	CSpace* GetViewSpace(void) const { return m_pViewSpace; }
	list<CObj*>& GetListObjs(void) { return m_listObjs; } // 객체의 생성과 소멸은 객체를 생성/소멸하는 주체가 담당한다.

private:
	CGraphicDevice* m_pGraphicDevice = nullptr;

	CCamera* m_pCamera			= nullptr;
	CTimer* m_pTimer			= nullptr;
	CSpace* m_pViewSpace		= nullptr;
	CSceneMgr* m_pSceneManager	= nullptr;
	list<CObj*> m_listObjs;
};

