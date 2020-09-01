#pragma once
#include "CSpriteObj.h"
class CDeco :
	public CSpriteObj
{
public:
	enum E_TYPE { TYPE_STONE, TYPE_TREE, TYPE_END };

public:
	CDeco(CGameWorld& _rGameWorld, float _fX, float _fY, CDeco::E_TYPE _eDecoType, int _iDecoID = -1);
	CDeco(CGameWorld& _rGameWorld, float _fX, float _fY, const wstring& _wstrDecoKey, int _iDecoID = -1);
	virtual ~CDeco();

private:
	CDeco::E_TYPE m_eDecoType = TYPE_END;
	int m_iDecoID = -1;
};

