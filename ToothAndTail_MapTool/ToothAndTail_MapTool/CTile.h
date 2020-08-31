#pragma once
#include "CSpriteObj.h"
class CTile :
	public CSpriteObj
{
public: 
	enum E_TYPE { TYPE_NORMAL, TYPE_BLOCKING, TYPE_OK, TYPE_NO, TYPE_END };

public:
	CTile(CGameWorld& _rGameWorld, float _fX, float _fY, CTile::E_TYPE = CTile::TYPE_NORMAL);
	virtual ~CTile();

private:
	CTile::E_TYPE m_eType = CTile::TYPE_NORMAL;
};

