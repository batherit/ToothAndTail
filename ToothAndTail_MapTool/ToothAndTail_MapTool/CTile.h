#pragma once
#include "CSpriteObj.h"
class CTile :
	public CSpriteObj
{
public: 
	enum E_TYPE { TYPE_NORMAL, TYPE_BLOCKING, TYPE_OK, TYPE_NO, TYPE_END };

public:
	CTile(CGameWorld& _rGameWorld, float _fX, float _fY, CTile::E_TYPE _eTileType = CTile::TYPE_NORMAL);
	virtual ~CTile();

	void SetTileType(CTile::E_TYPE _eTileType) { m_eTileType = _eTileType; SetAnimIndex(m_eTileType);}

private:
	CTile::E_TYPE m_eTileType = CTile::TYPE_NORMAL;
};

