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

	bool IsTileInRange(int iRow, int iCol) const;
	bool IsTileInRange(int iLineIndex) const;
	bool IsEmptyLot(const D3DXVECTOR3& _vPos, int _iRow, int _iCol, int _iPivotI, int _iPivotJ);
	const vector<CTile*>& GetTiles() { return m_vecTiles; }
	CTile* GetTile(D3DXVECTOR3 _vPos) const;
	CTile* GetTile(int _iRow, int _iCol) const;
	CTile* GetTile(int _iLineIndex) const;
	const vector<CDeco*>& GetDecos() { return m_vecDecos; }
	const vector<CTile*>& GetBlockingTiles() { return m_vecBlockingTiles; }
	const void PushObjectInMap(CObj* pObj);
	POINT GetRowColIndex(const D3DXVECTOR3& _vPos) const;
	POINT GetRowColIndex(int _iLineIndex) const;
	int GetLineIndex(const D3DXVECTOR3& _vPos) const;
	void UpdateBlockingTiles();
	D3DXVECTOR3 GetSiteCenter(const TileSiteInfo&  _rTileSiteInfo);
	void SetSiteType(const TileSiteInfo& _rTileSiteInfo, TILE::E_TYPE _eTileType);

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
	const int m_ciMapRow = 47;
	const int m_ciMapCol = 47;
	//int m_iMapCol = 1;
};

