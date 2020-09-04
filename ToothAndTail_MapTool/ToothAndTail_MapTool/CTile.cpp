#include "stdafx.h"
#include "CTile.h"
#include "CTextureMgr.h"



CTile::CTile(CGameWorld & _rGameWorld, float _fX, float _fY, TILE::E_TYPE _eTileType)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, TILE_WIDTH, TILE_HEIGHT),
	m_eTileType(_eTileType)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_NORMAL"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_BLOCKING"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_GREEN"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_RED"));

	SetAnimIndex(_eTileType);
}

CTile::CTile(CGameWorld & _rGameWorld)
	:
	CSpriteObj(_rGameWorld)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_NORMAL"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_BLOCKING"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_GREEN"));
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TILE_RED"));

	SetAnimIndex(0);
}

CTile::~CTile()
{
}

void CTile::SaveInfo(HANDLE & _hfOut)
{
	//DWORD dwByte = 0;
	WriteFile(_hfOut, &m_eTileType, sizeof(m_eTileType), nullptr, nullptr);
	WriteFile(_hfOut, &m_vPos, sizeof(m_vPos), nullptr, nullptr);
	WriteFile(_hfOut, &m_vScale, sizeof(m_vScale), nullptr, nullptr);
	POINT ptSize{ m_iWidth, m_iHeight };
	WriteFile(_hfOut, &ptSize, sizeof(ptSize), nullptr, nullptr);
}

void CTile::LoadInfo(HANDLE & _hfIn)
{
	ReadFile(_hfIn, &m_eTileType, sizeof(m_eTileType), nullptr, nullptr);
	ReadFile(_hfIn, &m_vPos, sizeof(m_vPos), nullptr, nullptr);
	ReadFile(_hfIn, &m_vScale, sizeof(m_vScale), nullptr, nullptr);
	POINT ptSize;
	ReadFile(_hfIn, &ptSize, sizeof(ptSize), nullptr, nullptr);

	SetAnimIndex(m_eTileType);
	m_iWidth = ptSize.x;
	m_iHeight = ptSize.y;
}

void CTile::PushOutOfTile(CObj* _pObj)
{
	if (m_eTileType != TILE::TYPE_BLOCKING) return;

	D3DXVECTOR3 vTilePos = GetXY();
	D3DXVECTOR3 vObjPos = _pObj->GetXY();
	if (!IsPointInTile(vObjPos, vTilePos, GetWidth() * fabs(GetScaleX() * 1.3f), GetHeight() * fabs(GetScaleY()* 1.3f))) return;

	// 4���� �������� ���Ѵ�.
	D3DXVECTOR3 vVertices[4] = {
		D3DXVECTOR3(vTilePos.x, vTilePos.y - (m_iHeight >> 1) * fabs(GetScaleY()* 1.3f), 0.f),
		D3DXVECTOR3(vTilePos.x + (m_iWidth >> 1) * fabs(GetScaleX()* 1.3f), vTilePos.y, 0.f),
		D3DXVECTOR3(vTilePos.x, vTilePos.y + (m_iHeight >> 1) * fabs(GetScaleY()* 1.3f), 0.f),
		D3DXVECTOR3(vTilePos.x - (m_iWidth >> 1) * fabs(GetScaleX()* 1.3f), vTilePos.y, 0.f)
	};

	D3DXVECTOR3 vToObj = vObjPos - vTilePos;
	int iLineIndex = 0;
	if (vToObj.x > 0.f) {
		if (vToObj.y <= 0.f) {
			iLineIndex = 0;
		}
		else {
			iLineIndex = 1;
		}
	}
	else {
		if (vToObj.y > 0.f) {
			iLineIndex = 2;
		}
		else {
			iLineIndex = 3;
		}
	}

	// ���� ���ϱ�
	float fGradient;
	fGradient = ((vVertices[(iLineIndex + 1) % 4].y - vVertices[iLineIndex % 4].y))
		/ (vVertices[(iLineIndex + 1) % 4].x - vVertices[iLineIndex % 4].x);

	// ���� ���ϱ� (b = y - ax)
	float fIntercept;
	fIntercept = vVertices[iLineIndex].y - fGradient * vVertices[iLineIndex].x;

	// ���� ���� ���ϱ�
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vNormal;
	vDir = vVertices[(iLineIndex + 1) % 4] - vVertices[iLineIndex % 4];
	D3DXVec3Normalize(&vDir, &vDir);
	vNormal = D3DXVECTOR3(vDir.y, -vDir.x, 0.f); // ������ ��ǥ�迡�� ������ �ٱ������� ���ϴ� ���� ����

	// ���� ����� ������ ã�´�.
	float fLength = 0.f;
	// (y =) ax + b => ax -y + b = 0;
	// ���� ���� ���� �Ÿ� �������� �Ÿ��� ���Ѵ�.
	fLength = fabs(fGradient * vObjPos.x - vObjPos.y + fIntercept)
		/ sqrtf(fGradient * fGradient + 1/*-1 * -1*/);
	// ���� ����(������ ����)������ �о��.
	_pObj->SetXY(vObjPos.x + vNormal.x * fLength, vObjPos.y + vNormal.y * fLength);
}
