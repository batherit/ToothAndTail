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
	D3DXMATRIX matScreen = _pCamera->GetScreenMatrix(GetWorldMatrix());
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);

	RECT rcAnimFrame;
	rcAnimFrame.left = GetAnimX();
	rcAnimFrame.top = GetAnimY();
	rcAnimFrame.right = rcAnimFrame.left + GetWidth();
	rcAnimFrame.bottom = rcAnimFrame.top + GetHeight();

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_vecTextureInfos[m_stAnimInfo.iAnimIndex]->pTexture, 
		&rcAnimFrame, 
		&D3DXVECTOR3(GetWidth() >> 1, GetHeight() >> 1, 0.f), 
		nullptr, 
		m_clRenderColor);
}

void CSpriteObj::RenderShadow(CCamera * _pCamera)
{
	D3DXMATRIX matWorld = GetWorldMatrix();
	matWorld._41 -= 50.f;
	matWorld._42 += 25.f;
	matWorld._21 += 1.5f;						// 전단
	matWorld._22 = matWorld._22 * 0.75f;		// 스케일 Y
	D3DXMATRIX matScreen = _pCamera->GetScreenMatrix(matWorld);

	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matScreen);

	RECT rcAnimFrame;
	rcAnimFrame.left = GetAnimX();
	rcAnimFrame.top = GetAnimY();
	rcAnimFrame.right = rcAnimFrame.left + GetWidth();
	rcAnimFrame.bottom = rcAnimFrame.top + GetHeight();

	CGraphicDevice::GetInstance()->GetSprite()->Draw(
		m_vecTextureInfos[m_stAnimInfo.iAnimIndex]->pTexture,
		&rcAnimFrame,
		&D3DXVECTOR3(GetWidth() >> 1, GetHeight() >> 1, 0.f),
		nullptr,
		D3DCOLOR_ARGB(122, 0, 0, 0,));
}

int CSpriteObj::UpdateAnim(float _fDeltaTime)
{
	// 반환값 내용
	// 0 : Anim이 정상적으로 업데이트 되었음. 무한 애님 포함.
	// 1 : 현재 Anim 상태가 무효화 상태이며 새로운 상태값 입력을 요구함.
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info를 갱신한다.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// 애니메이션 한바퀴를 다 돎.
		if (0 != m_stAnimInfo.iCountToRepeat) {
			// 무한 애님이 아니라면.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// 무한 애님이라면.
			m_stAnimProcessingInfo.fAnimElapsedTime = 0.f;
			m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
		}
	}

	float fCutTime = m_stAnimInfo.fTotalTime / m_stAnimInfo.iFrameCount;
	int iIndexOffset = static_cast<int>(m_stAnimProcessingInfo.fAnimElapsedTime / fCutTime);
	if (m_stAnimInfo.bIsReversePlay) iIndexOffset *= -1;
	m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex + iIndexOffset;

	return 0;
}
