#pragma once
#include "CSpriteObj.h"
class CTile :
	public CSpriteObj
{


public:
	CTile(CGameWorld& _rGameWorld, float _fX, float _fY, TILE::E_TYPE _eTileType = TILE::TYPE_NORMAL, int _iLineIndex = -1);
	CTile(CGameWorld& _rGameWorld, int iLineIndex = -1);	// 데이터 로드용
	virtual ~CTile();

public:
	virtual void SaveInfo(HANDLE& _hfOut);
	virtual void LoadInfo(HANDLE& _hfIn);
	virtual int Update(float _fDeltaTime) override;
	
public:
	void SetTileType(TILE::E_TYPE _eTileType) { m_eTileType = _eTileType; SetAnimIndex(m_eTileType);}
	TILE::E_TYPE GetTileType() const { return m_eTileType; }
	void PushOutOfTile(CObj* _pObj);
	void RegisterObjOnTile(CObj* _pObj) {
		DO_IF_IS_NOT_VALID_OBJ(_pObj) return;
		if (m_eTileType != TILE::TYPE_NORMAL) return;
		m_pDetectedObj = _pObj;
		SetColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	}
	int GetLineIndex() const { return m_iLineIndex; }

private:
	const int m_iLineIndex = -1;
	CObj* m_pDetectedObj = nullptr;
	TILE::E_TYPE m_eTileType = TILE::TYPE_NORMAL;
};

