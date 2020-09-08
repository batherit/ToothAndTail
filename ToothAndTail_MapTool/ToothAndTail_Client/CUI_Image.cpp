#include "stdafx.h"
#include "CUI_Image.h"
#include "CCamera.h"


CUI_Image::CUI_Image(CGameWorld & _rGameWorld, const TextureInfo * _pTextureInfo, const D3DXVECTOR3 & _vPos)
	:
	CObj(_rGameWorld, _vPos.x, _vPos.y, 10U, 10U)
{
	SetTextureInfo(_pTextureInfo);
	SetSize(_pTextureInfo->tImageInfo.Width, _pTextureInfo->tImageInfo.Height);
	if (_pTextureInfo) {
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
}

void CUI_Image::Render(CCamera * _pCamera)
{
	if (!m_pTextureInfo) return;

	// 스케일 조정
	//SetScaleX(static_cast<float>(m_rcOutputArea.right - m_rcOutputArea.left) / static_cast<float>(m_rcExtractionArea.right - m_rcOutputArea.left));
	//SetScaleY(static_cast<float>(m_rcOutputArea.bottom - m_rcOutputArea.top) / static_cast<float>(m_rcExtractionArea.bottom - m_rcOutputArea.top));

	D3DXMATRIX matScreen = GetObjectMatrix();
	// RenderOffset을 적용한다.
	matScreen._41 += GetRenderOffsetX();
	matScreen._42 += GetRenderOffsetY();
	if (_pCamera) matScreen = _pCamera->GetScreenMatrix(matScreen);

	// 컬링을 적용한다.
	RECT rcObjRect = GetRect();
	D3DXVECTOR3 vViewLeftTopW = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vViewRightBottomW = D3DXVECTOR3(static_cast<float>(WINCX), static_cast<float>(WINCY), 0.f);
	if (_pCamera) {
		vViewLeftTopW = _pCamera->GetWorldPoint(vViewLeftTopW);
		vViewRightBottomW = _pCamera->GetWorldPoint(vViewRightBottomW);
	}

	RECT rcViewRect = {
		static_cast<LONG>(vViewLeftTopW.x /*+ GetRenderOffsetX()*/),
		static_cast<LONG>(vViewLeftTopW.y /*+ GetRenderOffsetY()*/),
		static_cast<LONG>(vViewRightBottomW.x /*+ GetRenderOffsetX()*/),
		static_cast<LONG>(vViewRightBottomW.y /*+ GetRenderOffsetY()*/)
	};
	if (!IsCollided(rcViewRect, rcObjRect)) return;

	// 객체를 그린다.
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_pTextureInfo->pTexture,
		&m_rcExtractionArea,
		&D3DXVECTOR3(static_cast<FLOAT>(m_iWidth >> 1), static_cast<FLOAT>(m_iHeight >> 1), 0.f),
		nullptr,
		m_clRenderColor);
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
