#pragma once
#include "CSpriteObj.h"
class CTile :
	public CSpriteObj
{
public:
	CTile(CGameWorld& _rGameWorld, float _fX, float _fY, TILE::E_TYPE _eTileType = TILE::TYPE_NORMAL);
	CTile(CGameWorld& _rGameWorld);	// 데이터 로드용
	virtual ~CTile();

public:
	virtual void SaveInfo(HANDLE& _hfOut);
	virtual void LoadInfo(HANDLE& _hfIn);
	
public:
	void SetTileType(TILE::E_TYPE _eTileType) { m_eTileType = _eTileType; SetAnimIndex(m_eTileType);}
	TILE::E_TYPE GetTileType() const { return m_eTileType; }
	void PushOutOfTile(CObj* _pObj);

private:
	TILE::E_TYPE m_eTileType = TILE::TYPE_NORMAL;
};

