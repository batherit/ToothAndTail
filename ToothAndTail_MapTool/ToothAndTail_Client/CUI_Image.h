#pragma once
#include "CObj.h"
class CUI_Image :
	public CObj
{
public:
	CUI_Image(CGameWorld& _rGameWorld, const TextureInfo* _pTextureInfo, const D3DXVECTOR3& _vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	virtual ~CUI_Image();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

public:
	void SetOutputArea(const RECT& _rcOutputArea) { 
		m_rcOutputArea = _rcOutputArea;
		SetXY(static_cast<float>(_rcOutputArea.left + _rcOutputArea.right) * 0.5f, static_cast<float>(_rcOutputArea.top + _rcOutputArea.bottom) * 0.5f);
		m_fCorrectionRatioX = static_cast<float>(m_rcOutputArea.right - m_rcOutputArea.left) / (m_rcExtractionArea.right - m_rcExtractionArea.left);
		m_fCorrectionRatioY = static_cast<float>(m_rcOutputArea.bottom - m_rcOutputArea.top) / (m_rcExtractionArea.bottom - m_rcExtractionArea.top);
		SetSize((m_rcOutputArea.right - m_rcOutputArea.left), (m_rcOutputArea.bottom - m_rcOutputArea.top));
	}
	void SetOutputArea(LONG _lLeft, LONG _lTop, LONG _lRight, LONG _lBottom) {
		SetOutputArea(RECT({ _lLeft, _lTop, _lRight, _lBottom }));
	}
	void SetOutputArea(POINT _ptPos, size_t _iWidth, size_t _iHeight) {
		SetOutputArea(RECT({ 
			static_cast<LONG>(_ptPos.x - _iWidth * 0.5f),
			static_cast<LONG>(_ptPos.y - _iHeight * 0.5f),
			static_cast<LONG>(_ptPos.x + _iWidth * 0.5f),
			static_cast<LONG>(_ptPos.y + _iHeight * 0.5f)}));
	}
	void SetSyncOutputArea(const RECT& _rcOutputArea) {
		RECT rcNewExtractionArea = {
			static_cast<LONG>(m_rcExtractionArea.left + (_rcOutputArea.left - m_rcOutputArea.left) / m_fCorrectionRatioX) ,
			static_cast<LONG>(m_rcExtractionArea.top + (_rcOutputArea.top - m_rcOutputArea.top) / m_fCorrectionRatioY),
			static_cast<LONG>(m_rcExtractionArea.right + (_rcOutputArea.right - m_rcOutputArea.right) / m_fCorrectionRatioX),
			static_cast<LONG>(m_rcExtractionArea.bottom + (_rcOutputArea.bottom - m_rcOutputArea.bottom) / m_fCorrectionRatioY)
		};
		m_rcExtractionArea = rcNewExtractionArea;

		m_rcOutputArea = _rcOutputArea;
		SetXY(static_cast<float>(_rcOutputArea.left + _rcOutputArea.right) * 0.5f, static_cast<float>(_rcOutputArea.top + _rcOutputArea.bottom) * 0.5f);	
		SetSize((_rcOutputArea.right - _rcOutputArea.left), (_rcOutputArea.bottom - _rcOutputArea.top));
	}
	void SetExtractionArea(const RECT& _rcExractionArea) { 
		m_rcExtractionArea = _rcExractionArea;
		m_fCorrectionRatioX = static_cast<float>(m_rcOutputArea.right - m_rcOutputArea.left) / (m_rcExtractionArea.right - m_rcExtractionArea.left);
		m_fCorrectionRatioY = static_cast<float>(m_rcOutputArea.bottom - m_rcOutputArea.top) / (m_rcExtractionArea.bottom - m_rcExtractionArea.top);
		//SetSize((m_rcExtractionArea.right - m_rcExtractionArea.left), (m_rcExtractionArea.bottom - m_rcExtractionArea.top));
	}
	void SetExtractionArea(LONG _lLeft, LONG _lTop, LONG _lRight, LONG _lBottom) {
		SetExtractionArea(RECT({ _lLeft, _lTop, _lRight, _lBottom }));
	}

	// 테스트는 안해봤는데 잘될거임 'ㅅ';;
	void SetSyncExtractionArea(const RECT _rcExtractionArea) {
		RECT rcNewOutputArea = {
			static_cast<LONG>(m_rcOutputArea.left + (_rcExtractionArea.left - m_rcExtractionArea.left) * m_fCorrectionRatioX) ,
			static_cast<LONG>(m_rcOutputArea.top + (_rcExtractionArea.top - m_rcExtractionArea.top) * m_fCorrectionRatioY),
			static_cast<LONG>(m_rcOutputArea.right + (_rcExtractionArea.right - m_rcExtractionArea.right) * m_fCorrectionRatioX),
			static_cast<LONG>(m_rcOutputArea.bottom + (_rcExtractionArea.bottom - m_rcExtractionArea.bottom) * m_fCorrectionRatioY)
		};
		m_rcOutputArea = rcNewOutputArea;
		SetXY(static_cast<float>(rcNewOutputArea.left + rcNewOutputArea.right) * 0.5f, static_cast<float>(rcNewOutputArea.top + rcNewOutputArea.bottom) * 0.5f);
		SetSize((rcNewOutputArea.right - rcNewOutputArea.left), (rcNewOutputArea.bottom - rcNewOutputArea.top));

		m_rcExtractionArea = _rcExtractionArea;
	}
	

	void SetTextureInfo(const TextureInfo* _pTextureInfo) { if (_pTextureInfo) m_pTextureInfo = _pTextureInfo; }
	void SetRenderColor(const D3DXCOLOR& _clRenderColor) { m_clRenderColor = _clRenderColor; }

	RECT GetOutputArea(void) const { return m_rcOutputArea; }
	RECT GetExtractionArea(void) const { return m_rcExtractionArea; }

private:
	const TextureInfo* m_pTextureInfo = nullptr;
	RECT m_rcOutputArea;
	RECT m_rcExtractionArea;
	D3DXCOLOR m_clRenderColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	float m_fCorrectionRatioX = 1.f;
	float m_fCorrectionRatioY = 1.f;
	
};

