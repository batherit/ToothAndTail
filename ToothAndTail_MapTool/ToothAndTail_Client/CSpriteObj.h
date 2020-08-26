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
	// CObj��(��) ���� ��ӵ�
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

public:
	// ��Ʋ�� �ؽ�ó(�̱� �ؽ�ó�� ����)
	void PushTexture(const TextureInfo* _pTextureInfo) { if(_pTextureInfo) m_vecTextureInfos.emplace_back(_pTextureInfo); }
	void SetColor(const D3DCOLOR& _clRenderColor) { m_clRenderColor = _clRenderColor; }

	// �ִϸ��̼� ���� �Լ���
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) {
		m_stAnimInfo = _stAnimInfo;
		ZeroMemory(&m_stAnimProcessingInfo, sizeof(m_stAnimProcessingInfo));
	}
	virtual int UpdateAnim(float _fDeltaTime);
	AnimInfo& GetAnimInfo(void) { return m_stAnimInfo; }
	float GetAnimProgress(void) const { return m_stAnimProcessingInfo.fAnimElapsedTime / m_stAnimInfo.fTotalTime; }
	int GetAnimX(void) const { return (m_stAnimProcessingInfo.iCurrentIndex % m_stAnimInfo.iCol) * m_iWidth; }
	int GetAnimY(void) const { return (m_stAnimProcessingInfo.iCurrentIndex / m_stAnimInfo.iCol) * m_iHeight; }

private:
	vector<const TextureInfo*> m_vecTextureInfos;
	D3DCOLOR m_clRenderColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	AnimInfo m_stAnimInfo;
	AnimProcessingInfo m_stAnimProcessingInfo;
};

