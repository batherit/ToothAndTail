#pragma once

class CGameWorld;
class CMapObjsGroup;

class CScene abstract
{
public:
	CScene(CGameWorld& _rGameWorld);
	virtual ~CScene();

public:
	//virtual void Ready(void) = 0;
	virtual void ResetScene(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(HDC& _hdc, CCamera* _pCamera) {};
	virtual void Render(CCamera* _pCamera) {};
	virtual void Release(void) = 0;
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual list<CObj*>* GetHitEffects() { return nullptr; }
	virtual list<CObj*>* GetSpawners() { return nullptr; }
	virtual list<CObj*>* GetMonsters() { return nullptr; }
	virtual const vector<CMapObjsGroup*>* GetColliders() { return nullptr; }

protected:
	CGameWorld& m_rGameWorld;
};

