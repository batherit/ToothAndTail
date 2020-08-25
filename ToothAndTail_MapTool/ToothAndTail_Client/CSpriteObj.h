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

public:
	// 아틀라스 텍스처(싱글 텍스처의 일종)
	void PushTexture(const TextureInfo* _pTextureInfo) { if(_pTextureInfo) m_vecTextureInfos.emplace_back(_pTextureInfo); }
	void SetScaleX(float _fScaleX) { m_fScaleX = _fScaleX; }
	void SetScaleY(float _fScaleY) { m_fScaleY = _fScaleY; }
	void SetScaleXY(float _fScaleX, float _fScaleY) { SetScaleX(_fScaleX); SetScaleY(_fScaleY); }

	float GetScaleX(void) const { return m_fScaleX; }
	float GetScaleY(void) const { return m_fScaleY; }
	
	// 애니메이션 관련 함수들
	void SetNewAnimInfo(const AnimInfo& _stAnimInfo) {
		m_stAnimInfo = _stAnimInfo;
		ZeroMemory(&m_stAnimProcessingInfo, sizeof(m_stAnimProcessingInfo));
	}
	int UpdateAnim(float _fDeltaTime);
	AnimInfo& GetAnimInfo(void) { return m_stAnimInfo; }
	float GetAnimProgress(void) const { return m_stAnimProcessingInfo.fAnimElapsedTime / m_stAnimInfo.fTotalTime; }
	int GetAnimX(void) const { return (m_stAnimProcessingInfo.iCurrentIndex % m_stAnimInfo.iCol) * m_iWidth; }
	int GetAnimY(void) const { return (m_stAnimProcessingInfo.iCurrentIndex / m_stAnimInfo.iCol) * m_iHeight; }

private:
	vector<const TextureInfo*> m_vecTextureInfos;
	float m_fScaleX = 1.f;
	float m_fScaleY = 1.f;
	
	AnimInfo m_stAnimInfo;
	AnimProcessingInfo m_stAnimProcessingInfo;
};

