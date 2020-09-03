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
	void SaveInfo();
	void LoadInfo();

	void RenderMap();
	void RenderTiles();
	void RenderDecos();
	void RenderSelectedObj();

private:
	void SaveMapBorderLines(HANDLE& _hfOut);
	void SaveTiles(HANDLE& _hfOut);
	void SaveDecos(HANDLE& _hfOut);

	void ClearObjs();
	void LoadMapBorderLines(HANDLE& _hfIn);
	void LoadTiles(HANDLE& _hfIn);
	void LoadDecos(HANDLE& _hfIn);


	void LinkView(void);
	void LoadTextures(void);
	POINT GetRowColIndex(const D3DXVECTOR3& _vPos);
	int GetLineIndex(const D3DXVECTOR3& _vPos);

private:
	CCamera* m_pCamera = nullptr;
	CSpriteObj* m_pMap = nullptr;
	const D3DXVECTOR3 m_vStartPoint = D3DXVECTOR3(-10.f, (MAP_HEIGHT >> 1) * BASE_SCALE + 30.f, 0.f);
	D3DXVECTOR3 m_vMapBorderLines[4] = { //12시 방향부터 시계방향
		D3DXVECTOR3(1029.f * BASE_SCALE, 365.f* BASE_SCALE, 0.f),
		D3DXVECTOR3(2001.f* BASE_SCALE, 1022.f* BASE_SCALE, 0.f),
		D3DXVECTOR3(1029.f* BASE_SCALE, 1680.f* BASE_SCALE, 0.f),
		D3DXVECTOR3(42.5f* BASE_SCALE, 1022.f* BASE_SCALE, 0.f)
	};
	vector<CTile*> m_vecTiles;
	list<CDeco*> m_listDecos;
	int m_iMapRow = 1;
	int m_iMapCol = 1;

private:
	CForm* m_pForm = nullptr;
	CToothAndTailMapToolView* m_pToolView = nullptr;
};

