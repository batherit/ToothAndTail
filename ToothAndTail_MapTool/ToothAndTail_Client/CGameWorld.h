#pragma once

class CTimer;
class CSceneMgr;
class CSpace;
class CCamera;
class CObj;
class CGraphicDevice;
class CMapLoader;

class CGameWorld abstract
{
public:
	CGameWorld();
	virtual ~CGameWorld();

public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
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
	CMapLoader* SetMapLoader(CMapLoader* _pMapLoader) {
		CMapLoader* pOldMapLoader = m_pMapLoader_Main;
		m_pMapLoader_Main = _pMapLoader;
		return pOldMapLoader;
	}
	CMapLoader* GetMapLoader() const { return m_pMapLoader_Main; }

public:
	bool ConfirmValidScene(void);
	void RunTick(void);
	void StartRender(void);
	void EndRender(void);
	CTimer* GetTimer(void) const { return m_pTimer_Main; }
	CSceneMgr* GetSceneManager(void) const { return m_pSceneManager_Main; }
	CSpace* GetViewSpace(void) const { return m_pViewSpace_Main; }
	list<CObj*>& GetListObjs(void) { return m_listObjs_Main; } // 객체의 생성과 소멸은 객체를 생성/소멸하는 주체가 담당한다.
	vector<CObj*>& GetVecRenderObjs(void) { return m_vecRenderObjs_Main; }
	void RenderListObjs(CCamera* _pCamera, bool _bListClear = true, bool(*funcComp)(CObj* obj1, CObj* obj2) = [](CObj*, CObj*) { return false; });
	vector<CObj*>& GetRenderList() { return m_vecRenderObjs_Main; }

private:
	virtual void LoadResources() = 0;
	float m_fAnotherCameraKeepTime = 0.f;
	CTimer* m_pTimer_Main			= nullptr;

private:
	CGraphicDevice* m_pGraphicDevice = nullptr;

	CMapLoader* m_pMapLoader_Main	= nullptr;
	CCamera* m_pCamera_Main			= nullptr;
	
	CSpace* m_pViewSpace_Main		= nullptr;
	CSceneMgr* m_pSceneManager_Main	= nullptr;
	list<CObj*> m_listObjs_Main;
	vector<CObj*> m_vecRenderObjs_Main;
};

