#pragma once
#include "CObj.h"

class CTexture;
class CCamera;

class CSpriteObj :
	public CObj
{
public:
	CSpriteObj(CGameWorld& _rGameWorld, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f);
	virtual ~CSpriteObj();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;
	void SetShadow(bool _bIsRenderShadow, float _fInclination = 2.f, float _fScaleWeightY = 0.7f) {
		m_bIsToRenderShadow = _bIsRenderShadow;
		m_fInclination = _fInclination;
		m_fScaleWeightY = _fScaleWeightY;
	}

public:
	// 아틀라스 텍스처(싱글 텍스처의 일종)
	void PushTexture(const TextureInfo* _pTextureInfo) { if(_pTextureInfo) m_vecTextureInfos.emplace_back(_pTextureInfo); }
	void SetColor(const D3DCOLOR& _clRenderColor) { m_clRenderColor = _clRenderColor; }
	void SetShadowRender(bool _bIsToRenderShadow) { m_bIsToRenderShadow = _bIsToRenderShadow; }

	// 애니메이션 관련 함수들
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) {
		m_stAnimInfo = _stAnimInfo;
		ZeroMemory(&m_stAnimProcessingInfo, sizeof(m_stAnimProcessingInfo));
		m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
	}
	virtual int UpdateAnim(float _fDeltaTime);
	bool IsAnimPlaying(void) { return 0 == UpdateAnim(0.f); }
	AnimInfo& GetAnimInfo(void) { return m_stAnimInfo; }
	float GetAnimProgress(void) const { return m_stAnimProcessingInfo.fAnimElapsedTime / m_stAnimInfo.fTotalTime; }
	int GetAnimX(void) const { return (m_stAnimProcessingInfo.iCurrentIndex % m_stAnimInfo.iCol) * m_iWidth; }
	int GetAnimY(void) const { return (m_stAnimProcessingInfo.iCurrentIndex / m_stAnimInfo.iCol) * m_iHeight; }
	D3DCOLOR GetRenderColor(void) const { return m_clRenderColor; }

private:
	void RenderShadow(CCamera* _pCamera);

private:
	vector<const TextureInfo*> m_vecTextureInfos;
	D3DCOLOR m_clRenderColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	AnimInfo m_stAnimInfo;
	AnimProcessingInfo m_stAnimProcessingInfo;

	bool m_bIsToRenderShadow = false;
	float m_fInclination = 1.f;
	float m_fScaleWeightY = 1.f;
};

