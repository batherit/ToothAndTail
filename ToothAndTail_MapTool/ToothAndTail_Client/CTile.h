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
	virtual int Update(float _fDeltaTime) override;
	
public:
	void SetTileType(CTile::E_TYPE _eTileType) { m_eTileType = _eTileType; SetAnimIndex(m_eTileType);}
	CTile::E_TYPE GetTileType() const { return m_eTileType; }
	void PushOutOfTile(CObj* _pObj);
	void RegisterObjOnTile(CObj* _pObj) {
		DO_IF_IS_NOT_VALID_OBJ(_pObj) return;
		if (m_eTileType != CTile::TYPE_NORMAL) return;
		m_pDetectedObj = _pObj;
		SetColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	}

private:
	CObj* m_pDetectedObj = nullptr;
	CTile::E_TYPE m_eTileType = CTile::TYPE_NORMAL;
};

