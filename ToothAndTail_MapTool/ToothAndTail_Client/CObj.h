#pragma once

class CGameWorld;
class CCamera;

class CObj abstract
{
public:
	enum E_COORD_TYPE { COORD_TYPE_LOCAL, COORD_TYPE_WORLD, COORD_TYPE_END };

public:
	CObj(CGameWorld& _rGameWorld, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f);
	virtual ~CObj();
	
public: 
	virtual void Ready(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(HDC& _hdc, CCamera* _pCamera);
	virtual void Render(CCamera* _pCamera) {};
	// 조립 객체를 따로 따로 렌더링 하려는 경우.
	// 혜택. y값 정렬 렌더링
	virtual void RegisterToRenderList(vector<CObj*>& _vecRenderList) { _vecRenderList.emplace_back(this); } 
	virtual void Release(void) = 0;
	virtual void MakeModelVertices(void) {};

	// 파일 입출력 관련
	virtual void SaveInfo(FILE* _fpOut) {};
	virtual void LoadInfo(FILE* _fpIn) {};
	virtual void SaveInfo(HANDLE& _hfOut) {};
	virtual void LoadInfo(HANDLE& _hfIn) {};

	// 충돌 반응 관련
	virtual void ReactToCollider(CObj * _pCollider, RECT& _rcCollisionRect) {}; // 충돌체에 대한 반응을 정의해야함.

public:
	CGameWorld& GetGameWorld(void) const { return m_rGameWorld; }

public:
	void MoveTo(float _fDeltaX, float _fDeltaY) { m_vPos.x += _fDeltaX; m_vPos.y += _fDeltaY; /*if (m_pCollider) m_pCollider->LateUpdate();*/}
	void MoveByDeltaTime(float _fDeltaTime) { m_vPos.x += (m_vDir.x * m_fSpeed * _fDeltaTime); m_vPos.y += (m_vDir.y * m_fSpeed * _fDeltaTime); if (m_pCollider) m_pCollider->LateUpdate();}
	void RotateZ(float _fDeltaDegree) {
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(_fDeltaDegree));
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRotZ);
	}

	// Set
	void SetParent(CObj* _pParent) { m_pParent = _pParent; }
	void SetValid(bool _bIsValid) { m_bIsValid = _bIsValid; }
	void SetVisible(bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	//void SetModelType(OBJ::E_MODEL_TYPE _eModelType) { m_eModelType = _eModelType; }
	//void SetObjType(OBJ::E_TYPE _eObjType) { m_eObjType = _eObjType; }
	void SetRenderLayer(int _iRenderLayer) { m_iRenderLayer = _iRenderLayer; }
	void SetScale(float _fScale) { m_vScale.x = _fScale;  m_vScale.y = _fScale; }
	void SetScaleX(float _fScaleX) { m_vScale.x = _fScaleX; }
	void SetScaleY(float _fScaleY) { m_vScale.y = _fScaleY; }
	void SetScaleXY(float _fScaleX, float _fScaleY) { SetScaleX(_fScaleX); SetScaleY(_fScaleY); }
	void SetWidth(size_t _iWidth) { m_iWidth = _iWidth; }
	void SetHeight(size_t _iHeight) { m_iHeight = _iHeight; }
	void SetSize(size_t _iWidth, size_t _iHeight) { SetWidth(_iWidth); SetHeight(_iHeight); }
	void SetRenderOffsetX(float _fRenderOffsetX) { m_vRenderOffset.x = _fRenderOffsetX; }
	void SetRenderOffsetY(float _fRenderOffsetY) { m_vRenderOffset.y = _fRenderOffsetY; }
	void SetRenderOffsetXY(float _fRenderOffsetX, float _fRenderOffsetY) { 
		SetRenderOffsetX(_fRenderOffsetX); SetRenderOffsetY(_fRenderOffsetY);
	}
	void SetRenderOffsetXY(D3DXVECTOR3 _vRenderOffset) { m_vRenderOffset = _vRenderOffset; }
	void SetX(float _fX) { m_vPos.x = _fX;/* if (m_pCollider && (m_pCollider != this)) m_pCollider->LateUpdate();*/ }
	void SetY(float _fY) { m_vPos.y = _fY; /*if (m_pCollider && (m_pCollider != this)) m_pCollider->LateUpdate();*/ }
	void SetXY(float _fX, float _fY) { SetX(_fX); SetY(_fY); }
	void SetXY(D3DXVECTOR3 _vPos) { m_vPos = _vPos; }
	void SetToX(float _fToX) { m_vDir.x = _fToX; }
	void SetToY(float _fToY) { m_vDir.y = _fToY; }
	void SetToXY(float _fToX, float _fToY) { SetToX(_fToX); SetToY(_fToY); D3DXVec3Normalize(&m_vDir, &m_vDir); }
	void SetToXY(D3DXVECTOR3 _vDir) { m_vDir = _vDir; D3DXVec3Normalize(&m_vDir, &m_vDir); }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetRect(RECT _rcRect) {
		SetWidth(_rcRect.right - _rcRect.left);
		SetHeight(_rcRect.bottom - _rcRect.top);
		SetX(static_cast<float> ((_rcRect.right + _rcRect.left) >> 1));
		SetY(static_cast<float> ((_rcRect.bottom + _rcRect.top) >> 1));
	}
	
	// Get
	bool IsValid(void) const { return m_bIsValid; }
	bool IsVisible(void) const { return m_bIsVisible; }
	//OBJ::E_MODEL_TYPE GetModelType(void) const { return m_eModelType; }
	//const OBJ::E_TYPE GetObjType(void) const { return m_eObjType; }
	inline int GetRenderLayer(void) { return m_iRenderLayer; }
	float GetRenderOffsetX(void) const { return m_vRenderOffset.x; }
	float GetRenderOffsetY(void) const { return m_vRenderOffset.y; }
	D3DXVECTOR3 GetRenderOffsetXY(void) const { return D3DXVECTOR3(GetRenderOffsetX(), GetRenderOffsetY(), 0.f); }
	float GetX(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetXY(_eCoordType).x; }
	float GetY(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetXY(_eCoordType).y; }
	D3DXVECTOR3 GetXY(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { 
		D3DXVECTOR3 vPos = m_vPos;

		switch (_eCoordType) {
		case CObj::COORD_TYPE_WORLD:
			D3DXVec3TransformCoord(&vPos, &vPos, &GetParentMatrix(CObj::COORD_TYPE_WORLD));
			break;
		}

		return vPos;
	}
	float GetScaleX(void) const { return m_vScale.x; }
	float GetScaleY(void) const { return m_vScale.y; }
	size_t GetWidth(void) const { return m_iWidth; }
	size_t GetHeight(void) const { return m_iHeight; }
	float GetToX(void) const { return m_vDir.x; }
	float GetToY(void) const { return m_vDir.y; }
	D3DXVECTOR3 GetToXY(void) const { return D3DXVECTOR3(GetToX(), GetToY(), 0.f); }
	float GetSpeed(void) const { return m_fSpeed; }
	virtual float GetFloor() const { return GetBottom(); }

	inline RECT GetRect(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const {
		D3DXVECTOR3 vLeftTop(m_vPos.x + m_vRenderOffset.x - (m_iWidth >> 1) * fabs(GetScaleX()), m_vPos.y + m_vRenderOffset.y - (m_iHeight >> 1)* fabs(GetScaleY()), 0.f);
		D3DXVECTOR3 vRightBottom(m_vPos.x + m_vRenderOffset.x + (m_iWidth >> 1)* fabs(GetScaleX()), m_vPos.y + m_vRenderOffset.y + (m_iHeight >> 1)* fabs(GetScaleY()), 0.f);

		switch (_eCoordType) {
		case CObj::COORD_TYPE_WORLD:
			D3DXVec3TransformCoord(&vLeftTop, &vLeftTop, &GetParentMatrix(CObj::COORD_TYPE_WORLD));
			D3DXVec3TransformCoord(&vRightBottom, &vRightBottom, &GetParentMatrix(CObj::COORD_TYPE_WORLD));
			break;
		}
		
		return RECT({ static_cast<LONG>(vLeftTop.x), static_cast<LONG>(vLeftTop.y),
			static_cast<LONG>(vRightBottom.x), static_cast<LONG>(vRightBottom.y) });
	}
	inline LONG GetLeft(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetRect(_eCoordType).left; }
	inline LONG GetTop(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetRect(_eCoordType).top; }
	inline LONG GetRight(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetRect(_eCoordType).right; }
	inline LONG GetBottom(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const { return GetRect(_eCoordType).bottom; }

	CObj* GetCollider(void) const { return m_pCollider; }

	//D3DXMATRIX GetLocalMatrix(void) const;
	//D3DXMATRIX GetWorldMatrix(void) const;
	D3DXMATRIX GetParentMatrix(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const;
	D3DXMATRIX GetObjectMatrix(CObj::E_COORD_TYPE _eCoordType = CObj::COORD_TYPE_WORLD) const;
	
protected:
	CObj* m_pParent = nullptr;

	bool m_bIsValid = true;
	bool m_bIsVisible = true;

	// 모델 정점
	//OBJ::E_MODEL_TYPE m_eModelType = OBJ::MODEL_TYPE_END;
	//vector<D3DXVECTOR3> m_vecModelVertices;
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vScale = { 1.f, 1.f, 0.f };
	D3DXVECTOR3 m_vRenderOffset = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vDir = { 1.f, 0.f, 0.f };

	//OBJ::E_TYPE m_eObjType = OBJ::TYPE_END;
	float m_fSpeed = 0.f;
	size_t m_iWidth = 0;
	size_t m_iHeight = 0;
	int m_iRenderLayer = 0;

	CObj* m_pCollider = nullptr;

private:
	// CObj는 무조건 GameWorld 객체를 지녀야만 한다.
	CGameWorld& m_rGameWorld;
};

