#pragma once
#include "CSpriteObj.h"

class CFarmland;
class CCamera;
class CCrop :
	public CSpriteObj
{
public:
	CCrop(CGameWorld& _rGameWorld, CFarmland* _pFarmland, float _fX, float _fY, int _iCropIndex = 0);
	virtual ~CCrop();
};

