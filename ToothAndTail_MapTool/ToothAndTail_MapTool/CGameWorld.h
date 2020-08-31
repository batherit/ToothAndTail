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
	virtual ~CGameWorld();

public:
//	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Ready(void) = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(void) = 0;
	virtual void Release(void) = 0;
	CCamera* SetMainCamera(CCamera* _pCamera) { 
		CCamera* pOldCamera = m_pCamera_Main;
		m_pCamera_Main = _pCamera;
		return pOldCamera;
	}
	CCamera* GetMainCamera(void) const { return m_pCamera_Main; }

public:
	void RunTick(void);
	void StartRender(void);
	void EndRender(void);
	CTimer* GetTimer(void) const { return m_pTimer_Main; }
	CSpace* GetViewSpace(void) const { return m_pViewSpace_Main; }
	list<CObj*>& GetListObjs(void) { return m_listObjs_Main; } // 객체의 생성과 소멸은 객체를 생성/소멸하는 주체가 담당한다.
	vector<CObj*>& GetVecRenderObjs(void) { return m_vecRenderObjs_Main; }
	void RenderListObjs(CCamera* _pCamera, bool(*funcComp)(CObj* obj1, CObj* obj2) = [](CObj*, CObj*) { return false; });

private:
	CGraphicDevice* m_pGraphicDevice = nullptr;

	CCamera* m_pCamera_Main			= nullptr;
	CTimer* m_pTimer_Main			= nullptr;
	CSpace* m_pViewSpace_Main		= nullptr;
	list<CObj*> m_listObjs_Main;
	vector<CObj*> m_vecRenderObjs_Main;
};

