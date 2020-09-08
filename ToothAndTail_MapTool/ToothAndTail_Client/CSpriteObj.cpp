#include "stdafx.h"
#include "CSpriteObj.h"
#include "CTexture.h"
#include "CCamera.h"


CSpriteObj::CSpriteObj(CGameWorld & _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, float _fToX, float _fToY, float _fSpeed)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fToX, _fToY, _fSpeed)
{
}

CSpriteObj::~CSpriteObj()
{
}

void CSpriteObj::Ready(void)
{
}

int CSpriteObj::Update(float _fDeltaTime)
{
	return UpdateAnim(_fDeltaTime);
}

void CSpriteObj::LateUpdate(void)
{
}

void CSpriteObj::Release(void)
{
	m_vecTextureInfos.clear();
	m_vecTextureInfos.shrink_to_fit();
}

void CSpriteObj::Render(CCamera * _pCamera)
{
	if (m_vecTextureInfos.empty()) return;
	
	D3DXMATRIX matScreen = GetObjectMatrix();
	// RenderOffset�� �����Ѵ�.
	matScreen._41 += GetRenderOffsetX();
	matScreen._42 += GetRenderOffsetY();
	if(_pCamera) matScreen = _pCamera->GetScreenMatrix(matScreen);

	// �ø��� �����Ѵ�.
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

	// �׸��ڸ� �׸���.
	if (m_bIsToRenderShadow) RenderShadow(_pCamera);

	// ��ü�� �׸���.
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);
	RECT rcAnimFrame;
	rcAnimFrame.left = GetAnimX();
	rcAnimFrame.top = GetAnimY();
	rcAnimFrame.right = rcAnimFrame.left + GetWidth();
	rcAnimFrame.bottom = rcAnimFrame.top + GetHeight();

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_vecTextureInfos[m_stAnimInfo.iAnimIndex]->pTexture, 
		&rcAnimFrame, 
		&D3DXVECTOR3(static_cast<FLOAT>(m_iWidth >> 1), static_cast<FLOAT>(m_iHeight >> 1), 0.f),
		nullptr, 
		m_clRenderColor);

	// ��Ʈ ǥ�ÿ�.
	/*rcObjRect = GetRect();
	D3DXVECTOR3 vLeftTop = _pCamera->GetScreenPoint(D3DXVECTOR3(static_cast<FLOAT>(rcObjRect.left), static_cast<FLOAT>(rcObjRect.top), 0.f));
	D3DXVECTOR3 vRightBottom = _pCamera->GetScreenPoint(D3DXVECTOR3(static_cast<FLOAT>(rcObjRect.right), static_cast<FLOAT>(rcObjRect.bottom), 0.f));
	D3DXVECTOR2 vecList[] =
	{
	   D3DXVECTOR2(vLeftTop.x, vLeftTop.y),
	   D3DXVECTOR2(vLeftTop.x, vRightBottom.y),
	   D3DXVECTOR2(vRightBottom.x, vRightBottom.y),
	   D3DXVECTOR2(vRightBottom.x, vLeftTop.y),
	   D3DXVECTOR2(vLeftTop.x, vLeftTop.y)
	};

	LPD3DXLINE pLine;
	D3DXCreateLine(CGraphicDevice::GetInstance()->GetDevice(), &pLine);
	pLine->Begin();
	pLine->Draw(vecList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	pLine->End();
	pLine->Release();*/
}

void CSpriteObj::RenderShadow(CCamera * _pCamera)
{
	float fParentAbsScaleY = 1.f;
	if (m_pParent) {
		fParentAbsScaleY = fabs(m_pParent->GetScaleY());
	}

	D3DXMATRIX matWorld = GetObjectMatrix();
	matWorld._21 = m_fInclination;			// ����
	matWorld._22 *= m_fScaleWeightY;			// ������ Y
	matWorld._41 += GetRenderOffsetX() + (m_iWidth >> 1) * -m_fInclination;
	matWorld._42 += GetRenderOffsetY() + (m_iHeight >> 1) * fParentAbsScaleY * fabs(GetScaleY()) * (1.f - m_fScaleWeightY);
	//matWorld._41 += /*(-GetPivotX() + (m_iWidth >> 1)) * fabs(GetScaleX()) * fabs(fParentScaleX)*/ ;
	//matWorld._42 += /*(-GetPivotY() + (m_iHeight >> 1)) * fabs(GetScaleY()) * fabs(fParentScaleY)*/ + (m_iHeight >> 1)* fabs(GetScaleY())* fabs(fParentScaleY) * (1.f - _fScaleWeightY);
	D3DXMATRIX matScreen = _pCamera->GetScreenMatrix(matWorld);

	// �ø��� �����Ѵ�. (���� ��ǥ���� ��) => ��ü Render���� üũ�ϱ� ������ ���� �ʿ�� ����.
	//RECT rcObjRect = GetRect();
	//D3DXVECTOR3 vViewLeftTopW = _pCamera->GetWorldPoint(D3DXVECTOR3(0.f, 0.f, 0.f));
	//D3DXVECTOR3 vViewRightBottomW = _pCamera->GetWorldPoint(D3DXVECTOR3(static_cast<float>(WINCX), static_cast<float>(WINCY), 0.f));
	//RECT rcViewRect = {
	//	static_cast<LONG>(vViewLeftTopW.x /*+ GetRenderOffsetX()*/),
	//	static_cast<LONG>(vViewLeftTopW.y /*+ GetRenderOffsetY()*/),
	//	static_cast<LONG>(vViewRightBottomW.x /*+ GetRenderOffsetX()*/),
	//	static_cast<LONG>(vViewRightBottomW.y /*+ GetRenderOffsetY()*/)
	//};
	//if (!IsCollided(rcViewRect, rcObjRect)) return;

	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);

	RECT rcAnimFrame;
	rcAnimFrame.left = GetAnimX();
	rcAnimFrame.top = GetAnimY();
	rcAnimFrame.right = rcAnimFrame.left + GetWidth();
	rcAnimFrame.bottom = rcAnimFrame.top + GetHeight();

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_vecTextureInfos[m_stAnimInfo.iAnimIndex]->pTexture,
		&rcAnimFrame,
		&D3DXVECTOR3(static_cast<FLOAT>(m_iWidth >> 1), static_cast<FLOAT>(m_iHeight >> 1), 0.f),
		nullptr,
		D3DCOLOR_ARGB(122, 0, 0, 0));

	/*RECT rcObjRect = GetRect();
	D3DXVECTOR3 vLeftTop = _pCamera->GetScreenPoint(D3DXVECTOR3(static_cast<FLOAT>(rcObjRect.left), static_cast<FLOAT>(rcObjRect.top), 0.f));
	D3DXVECTOR3 vRightBottom = _pCamera->GetScreenPoint(D3DXVECTOR3(static_cast<FLOAT>(rcObjRect.right), static_cast<FLOAT>(rcObjRect.bottom), 0.f));
	D3DXVECTOR2 vecList[] =
	{
	   D3DXVECTOR2(vLeftTop.x, vLeftTop.y),
	   D3DXVECTOR2(vLeftTop.x, vRightBottom.y),
	   D3DXVECTOR2(vRightBottom.x, vRightBottom.y),
	   D3DXVECTOR2(vRightBottom.x, vLeftTop.y),
	   D3DXVECTOR2(vLeftTop.x, vLeftTop.y)
	};

	LPD3DXLINE pLine;
	D3DXCreateLine(CGraphicDevice::GetInstance()->GetDevice(), &pLine);
	pLine->Begin();
	pLine->Draw(vecList, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	pLine->End();
	pLine->Release();*/
}

int CSpriteObj::UpdateAnim(float _fDeltaTime)
{
	// ��ȯ�� ����
	// 0 : Anim�� ���������� ������Ʈ �Ǿ���. ���� �ִ� ����.
	// 1 : ���� Anim ���°� ��ȿȭ �����̸� ���ο� ���°� �Է��� �䱸��.
	if (m_stAnimInfo.iFrameCount == 0) return 1;
	if (m_stAnimInfo.iFrameCount == 1) return 0;
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info�� �����Ѵ�.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// �ִϸ��̼� �ѹ����� �� ��.
		if (0 != m_stAnimInfo.iCountToRepeat) {
			// ���� �ִ��� �ƴ϶��.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// ���� �ִ��̶��.	
			m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
		}
		m_stAnimProcessingInfo.fAnimElapsedTime = 0.f;
	}

	float fCutTime = m_stAnimInfo.fTotalTime / m_stAnimInfo.iFrameCount;
	int iIndexOffset = static_cast<int>(m_stAnimProcessingInfo.fAnimElapsedTime / fCutTime);
	if (m_stAnimInfo.bIsReversePlay) iIndexOffset *= -1;
	m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex + iIndexOffset;

	return 0;
}
