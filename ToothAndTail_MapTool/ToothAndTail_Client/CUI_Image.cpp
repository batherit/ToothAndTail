#include "stdafx.h"
#include "CUI_Image.h"
#include "CCamera.h"


CUI_Image::CUI_Image(CGameWorld & _rGameWorld, const TextureInfo * _pTextureInfo, const D3DXVECTOR3 & _vPos)
	:
	CObj(_rGameWorld, _vPos.x, _vPos.y, 10U, 10U)
{
	if (_pTextureInfo) {
		SetTextureInfo(_pTextureInfo);
		SetSize(_pTextureInfo->tImageInfo.Width, _pTextureInfo->tImageInfo.Height);
		SetExtractionArea(RECT({ 0, 0, static_cast<LONG>(m_iWidth), static_cast<LONG>(m_iHeight) }));
		SetOutputArea(RECT(
			{
				static_cast<LONG>(_vPos.x - m_rcExtractionArea.right * 0.5f),
				static_cast<LONG>(_vPos.y - m_rcExtractionArea.bottom * 0.5f),
				static_cast<LONG>(_vPos.x + m_rcExtractionArea.right * 0.5f),
				static_cast<LONG>(_vPos.y + m_rcExtractionArea.bottom * 0.5f)
			}));
	}
}

CUI_Image::~CUI_Image()
{
	m_pTextureInfo = nullptr;
}

void CUI_Image::Render(CCamera * _pCamera)
{
	if (!m_pTextureInfo) return;

	D3DXMATRIX matScreen;
	// 보정 비율을 곱한다.
	D3DXMatrixScaling(&matScreen, m_fCorrectionRatioX, m_fCorrectionRatioY, 0.f);
	matScreen *= GetObjectMatrix();
	// RenderOffset을 적용한다.
	matScreen._41 += GetRenderOffsetX();
	matScreen._42 += GetRenderOffsetY();
	if (_pCamera) matScreen = _pCamera->GetScreenMatrix(matScreen);

	// 컬링을 적용한다.
	if (!IsObjInCamera(this, _pCamera)) return;

	// 객체를 그린다.
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_pTextureInfo->pTexture,
		&m_rcExtractionArea,
		&D3DXVECTOR3(
			static_cast<FLOAT>((m_rcExtractionArea.right - m_rcExtractionArea.left) >> 1), 
			static_cast<FLOAT>((m_rcExtractionArea.bottom - m_rcExtractionArea.top) >> 1),
			0.f),
		nullptr, m_clRenderColor);
}

void CUI_Image::Ready(void)
{
}

int CUI_Image::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_Image::LateUpdate(void)
{
}

void CUI_Image::Release(void)
{
}
