#pragma once
#include "CSpriteObj.h"
class CTile :
	public CSpriteObj
{
public: 
	enum E_TYPE { TYPE_NORMAL, TYPE_BLOCKING, TYPE_OK, TYPE_NO, TYPE_END };

public:
	CTile(CGameWorld& _rGameWorld, float _fX, float _fY, CTile::E_TYPE _eTileType = CTile::TYPE_NORMAL);
	CTile(CGameWorld& _rGameWorld);	// 데이터 로드용
	virtual ~CTile();

public:
	virtual void SaveInfo(HANDLE& _hfOut);
	virtual void LoadInfo(HANDLE& _hfIn);
	
public:
	void SetTileType(CTile::E_TYPE _eTileType) { m_eTileType = _eTileType; SetAnimIndex(m_eTileType);}

private:
	CTile::E_TYPE m_eTileType = CTile::TYPE_NORMAL;
};

