#pragma once
class CGraphicDevice
{
	DECLARE_SINGLETON(CGraphicDevice)

	// 싱글톤 클래스이므로 생성자 접근 제한자를 private으로 둠.
private:
	CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
	LPD3DXSPRITE GetSprite() { return m_pSprite; }
	LPD3DXFONT GetFont() { return m_pFont; }

public:
	void RenderText(const wstring& _wstrText, D3DXVECTOR3 _vPos, float _fScale = 1.f, D3DXCOLOR _clRenderColor = D3DCOLOR_ARGB(255, 255, 255, 255));
	HRESULT GenerateGraphicDevice();
	void ReleaseGraphicDevice();

	void BeginRender();
	void EndRender(HWND hWnd = nullptr);

private:
	LPDIRECT3D9 m_pSDK;					// 장치의 지원 수준 조사 및 장치를 제어할 객체를 생성할 com 객체
	LPDIRECT3DDEVICE9 m_pDevice;		// 실질적으로 장치를 다룰 객체

	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
};

