#pragma once
class CGameWorld;
class CSpriteObj;
class CTile;
class CDeco;
class CMapLoader final
{
public:
	explicit CMapLoader(CGameWorld& _rGameWorld, const wstring& wstrMapFilePath);
	virtual ~CMapLoader();
	
public:
	void Release(void);

public:
	void RenderMap(CCamera* _pCamera);
	void RenderTile(CCamera* _pCamera);

	const vector<CTile*>& GetTiles(void) { return m_vecTiles; }
	const vector<CDeco*>& GetDecos(void) { return m_vecDecos; }

private:
	void ClearObjs();
	void LoadMapBorderLines(HANDLE& _hfIn);
	void LoadTiles(HANDLE& _hfIn);
	void LoadDecos(HANDLE& _hfIn);

private:
	CGameWorld& m_rGameWorld;
	CSpriteObj* m_pMap = nullptr;
	D3DXVECTOR3 m_vMapBorderLines[4];
	vector<CTile*> m_vecTiles;
	vector<CTile*> m_vecBlockingTiles;
	vector<CDeco*> m_vecDecos;
	//int m_iMapRow = 1;
	//int m_iMapCol = 1;
};

