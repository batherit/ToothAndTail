#pragma once
#include "CGameWorld.h"
class CSpriteObj;
class CTile;
class CDeco;
class CForm;
class CToothAndTailMapToolView;
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

public:
	void OnLButtonDown(UINT nFlags, CPoint point);

public:
	void RenderMap();
	void RenderTiles();
	void RenderDecos();
	void RenderSelectedObj();

private:
	void LinkView(void);
	void LoadTextures(void);

private:
	CCamera* m_pCamera = nullptr;
	CSpriteObj* m_pMap = nullptr;
	const D3DXVECTOR3 m_MapBorderLines[4] = { //12시 방향부터 시계방향
		D3DXVECTOR3(1029.f, 365.f, 0.f),
		D3DXVECTOR3(2001.f, 1022.f, 0.f),
		D3DXVECTOR3(1029.f, 1680.f, 0.f),
		D3DXVECTOR3(42.5f, 1022.f, 0.f)
	};
	list<CTile*> m_listTiles;
	list<CDeco*> m_listDecos;
	int m_iMapRow = 1;
	int m_iMapCol = 1;

private:
	CForm* m_pForm = nullptr;
	CToothAndTailMapToolView* m_pToolView = nullptr;
};

