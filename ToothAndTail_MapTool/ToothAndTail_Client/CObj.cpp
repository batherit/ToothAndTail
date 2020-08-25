#include "stdafx.h"
#include "CObj.h"


CObj::CObj(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)

{
}

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

		D3DXMATRIX matWorld, matScale, matTrans, matRotZ;		// ����/�̵�/ȸ�����
		D3DXMatrixIdentity(&matWorld);				// �̵������ �׵���ķ� �ʱ�ȭ

		// ��������� �ۼ�
		D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, 0.f);

		// �̵���� �ۼ�
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, 0.f);	// ��ü ��ġ ��ǥ�κ��� ����� ����.

		// ȸ����� �ۼ�
		float fRotDegree = GetPositiveDegreeByVector(m_vDir.x, m_vDir.y);	// ��ü ���� ���ͷκ��� ����� ����.
		D3DXMatrixRotationZ(&matRotZ, TO_RADIAN(fRotDegree));

		// ���� ��� �ۼ�
		matWorld = matScale * matRotZ * matTrans;

		// ����(��ġ����)�� ��������� ������.
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

void CObj::SaveInfo(FILE * _fpOut)
{
}

void CObj::LoadInfo(FILE * _fpIn)
{
}