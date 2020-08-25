#pragma once
#include "CObj.h"

class CTexture;
class CSpriteObj :
	public CObj
{
public:
	CSpriteObj(CGameWorld& _rGameWorld, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f);
	virtual ~CSpriteObj();

public:


protected:
	vector<CTexture*> m_vecTextures;
};

