#pragma once

class CGameWorld;
class CCamera;

class CObj abstract
{
public:
	CObj(CGameWorld& _rGameWorld);
	CObj(CGameWorld& _rGameWorld, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f);
	virtual ~CObj();
	
public: 
	virtual void Ready(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(HDC& _hdc, CCamera* _pCamera);
	virtual void Release(void) = 0;
	virtual void MakeModelVertices(void) {};

	// ���� ����� ����
	virtual void SaveInfo(FILE* _fpOut);
	virtual void LoadInfo(FILE* _fpIn);

	// �浹 ���� ����
	virtual void ReactToCollider(CObj * _pCollider, RECT& _rcCollisionRect) {}; // �浹ü�� ���� ������ �����ؾ���.

public:
	CGameWorld& GetGameWorld(void) const { return m_rGameWorld; }

public:
	void MoveTo(float _fDeltaX, float _fDeltaY) { m_vPos.x += _fDeltaX; m_vPos.y += _fDeltaY; if (m_pCollider) m_pCollider->LateUpdate();}
	void MoveByDeltaTime(float _fDeltaTime) { m_vPos.x += (m_vDir.x * m_fSpeed * _fDeltaTime); m_vPos.y += (m_vDir.y * m_fSpeed * _fDeltaTime); if (m_pCollider) m_pCollider->LateUpdate();}
	void RotateZ(float _fDeltaDegree) {
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(_fDeltaDegree));
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRotZ);
	}

	// Set
	void SetValid(bool _bIsValid) { m_bIsValid = _bIsValid; }
	void SetVisible(bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	//void SetModelType(OBJ::E_MODEL_TYPE _eModelType) { m_eModelType = _eModelType; }
	//void SetObjType(OBJ::E_TYPE _eObjType) { m_eObjType = _eObjType; }
	void SetRenderLayer(int _iRenderLayer) { m_iRenderLayer = _iRenderLayer; }
	void SetScale(float _fScale) { m_fScaleX = _fScale; m_fScaleY = _fScale; }
	void SetScaleX(float _fScaleX) { m_fScaleX = _fScaleX; }
	void SetScaleY(float _fScaleY) { m_fScaleY = _fScaleY; }
	void SetWidth(size_t _iWidth) { m_iWidth = _iWidth; }
	void SetHeight(size_t _iHeight) { m_iHeight = _iHeight; }
	void SetX(float _fX) { m_vPos.x = _fX; if (m_pCollider && (m_pCollider != this)) m_pCollider->LateUpdate(); }
	void SetY(float _fY) { m_vPos.y = _fY; if (m_pCollider && (m_pCollider != this)) m_pCollider->LateUpdate(); }
	void SetXY(float _fX, float _fY) { SetX(_fX); SetY(_fY); }
	void SetToX(float _fToX) { m_vDir.x = _fToX; }
	void SetToY(float _fToY) { m_vDir.y = _fToY; }
	void SetToXY(float _fToX, float _fToY) { SetToX(_fToX); SetToY(_fToY); D3DXVec3Normalize(&m_vDir, &m_vDir); }
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
	int GetRenderLayer(void) { return m_iRenderLayer; }
	float GetX(void) const { return m_vPos.x; }
	float GetY(void) const { return m_vPos.y; }
	float GetScaleX(void) const { return m_fScaleX; }
	float GetScaleY(void) const { return m_fScaleY; }
	size_t GetWidth(void) const { return m_iWidth; }
	size_t GetHeight(void) const { return m_iHeight; }
	D3DXVECTOR3 GetXY(void) const { return D3DXVECTOR3(GetX(), GetY(), 0.f); }
	float GetToX(void) const { return m_vDir.x; }
	float GetToY(void) const { return m_vDir.y; }
	D3DXVECTOR3 GetToXY(void) const { return D3DXVECTOR3(GetToX(), GetToY(), 0.f); }
	float GetSpeed(void) const { return m_fSpeed; }
	LONG GetLeft(void) const { return static_cast<int>(m_vPos.x - (m_iWidth >> 1)); }
	LONG GetTop(void) const { return static_cast<int>(m_vPos.y - (m_iHeight >> 1)); }
	LONG GetRight(void) const { return static_cast<int>(m_vPos.x + (m_iWidth >> 1)); }
	LONG GetBottom(void) const { return static_cast<int>(m_vPos.y + (m_iHeight >> 1)); }
	RECT GetRect(void) const { 
		RECT rc = { 
		static_cast<LONG>(m_vPos.x - (m_iWidth >> 1)),
		static_cast<LONG>(m_vPos.y - (m_iHeight >> 1)),
		static_cast<LONG>(m_vPos.x + (m_iWidth >> 1)),
		static_cast<LONG>(m_vPos.y + (m_iHeight >> 1))
		}; 
		return rc; 
	}

	CObj* GetCollider(void) const { return m_pCollider; }
	
protected:
	bool m_bIsValid = true;
	bool m_bIsVisible = true;

	// �� ����
	//OBJ::E_MODEL_TYPE m_eModelType = OBJ::MODEL_TYPE_END;
	//vector<D3DXVECTOR3> m_vecModelVertices;
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vDir = { 1.f, 0.f, 0.f };

	//OBJ::E_TYPE m_eObjType = OBJ::TYPE_END;
	float m_fSpeed = 0.f;
	float m_fScaleX = 1.f;
	float m_fScaleY = 1.f;
	size_t m_iWidth = 0;
	size_t m_iHeight = 0;
	int m_iRenderLayer = 0;

	CObj* m_pCollider = nullptr;

private:
	// CObj�� ������ GameWorld ��ü�� ����߸� �Ѵ�.
	CGameWorld& m_rGameWorld;
};
