#include "stdafx.h"
#include "CObj.h"



CObj::CObj(CGameWorld& _rGameWorld, float _fX /*= 0*/, float _fY /*= 0*/, size_t _iWidth /*= 10*/, size_t _iHeight /*= 10*/, float _fToX, float _fToY, float _fSpeed /*= 0.f*/)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsValid(true),
	m_vPos(D3DXVECTOR3(_fX, _fY, 0.f)),
	m_vDir(D3DXVECTOR3(_fToX, _fToY, 0.f)),
	m_iWidth(_iWidth),
	m_iHeight(_iHeight),
	m_fSpeed(_fSpeed)
{
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vPivot = D3DXVECTOR3(static_cast<FLOAT>(m_iWidth >> 1), static_cast<FLOAT>(m_iHeight >> 1), 0.f);
}


CObj::~CObj()
{
}

void CObj::Render(HDC & _hdc, CCamera * _pCamera)
{
	/*switch (m_eModelType)
	{
	case OBJ::MODEL_TYPE_ELLIPSE: {
		RECT& rcObjRect = GetRect();
		rcObjRect.left = (rcObjRect.left - GetX()) * m_fScaleX + GetX();
		rcObjRect.top = (rcObjRect.top - GetY()) * m_fScaleY + GetY();
		rcObjRect.right = (rcObjRect.right - GetX()) * m_fScaleX + GetX();
		rcObjRect.bottom = (rcObjRect.bottom - GetY()) * m_fScaleY + GetY();

		Ellipse(_hdc, rcObjRect.left, rcObjRect.top, rcObjRect.right, rcObjRect.bottom);
	}
		break;
	case OBJ::MODEL_TYPE_RECTANGLE: {
		RECT& rcObjRect = GetRect();
		rcObjRect.left = (rcObjRect.left - GetX()) * m_fScaleX + GetX();
		rcObjRect.top = (rcObjRect.top - GetY()) * m_fScaleY + GetY();
		rcObjRect.right = (rcObjRect.right - GetX()) * m_fScaleX + GetX();
		rcObjRect.bottom = (rcObjRect.bottom - GetY()) * m_fScaleY + GetY();

		Rectangle(_hdc, rcObjRect.left, rcObjRect.top, rcObjRect.right, rcObjRect.bottom);
	}
		break;
	case OBJ::MODEL_TYPE_LINE: {
		if (m_vecModelVertices.empty()) return;

		D3DXMATRIX matWorld, matScale, matTrans, matRotZ;		// 월드/이동/회전행렬
		D3DXMatrixIdentity(&matWorld);				// 이동행렬을 항등행렬로 초기화

		// 스케일행렬 작성
		D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, 0.f);

		// 이동행렬 작성
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, 0.f);	// 객체 위치 좌표로부터 행렬을 만듦.

		// 회전행렬 작성
		float fRotDegree = GetPositiveDegreeByVector(m_vDir.x, m_vDir.y);	// 객체 방향 벡터로부터 행렬을 만듦.
		D3DXMatrixRotationZ(&matRotZ, TO_RADIAN(fRotDegree));

		// 월드 행렬 작성
		matWorld = matScale * matRotZ * matTrans;

		// 정점(위치벡터)에 월드행렬을 적용함.
		D3DXVECTOR3 vConverted;
		D3DXVec3TransformCoord(&vConverted, &m_vecModelVertices[0], &matWorld);
		MoveToEx(_hdc, vConverted.x, vConverted.y, NULL);
		int iVecSize = m_vecModelVertices.size();
		for (int i = 1; i < iVecSize; i++) {
			D3DXVec3TransformCoord(&vConverted, &m_vecModelVertices[i], &matWorld);
			LineTo(_hdc, vConverted.x, vConverted.y);
		}
	}
		break;
	}*/
}

D3DXMATRIX CObj::GetParentMatrix(CObj::E_COORD_TYPE _eCoordType) const
{
	D3DXMATRIX matParent;
	D3DXMatrixIdentity(&matParent);

	if(m_pParent) 
		matParent = m_pParent->GetObjectMatrix(_eCoordType);

	return matParent;
}

D3DXMATRIX CObj::GetObjectMatrix(CObj::E_COORD_TYPE _eCoordType) const
{
	D3DXMATRIX matObj, matScale, matTrans;
	D3DXMatrixIdentity(&matObj);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, 0.f);	//Local
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, 0.f);	//Local
	matObj = matScale * matTrans;

	switch (_eCoordType)
	{
	case CObj::COORD_TYPE_WORLD: 
		matObj *= GetParentMatrix(_eCoordType);
		break;
	}

	return matObj;
}

//D3DXMATRIX CObj::GetLocalMatrix(void) const
//{
//	D3DXMATRIX matScale, matTrans;
//	D3DXMatrixScaling(&matScale, GetScaleX(), GetScaleY(), 0.f);
//	D3DXMatrixTranslation(&matTrans, GetX(), GetY(), 0.f);
//
//	return matScale * matTrans;
//}
//
//D3DXMATRIX CObj::GetWorldMatrix(void) const
//{
//	D3DXMATRIX matParentW;
//	D3DXMatrixIdentity(&matParentW);
//
//	if (m_pParent) matParentW = m_pParent->GetWorldMatrix();
//
//	return GetLocalMatrix() * matParentW;
//}
