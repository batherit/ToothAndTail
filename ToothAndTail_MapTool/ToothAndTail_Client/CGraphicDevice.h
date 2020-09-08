#pragma once
class CGraphicDevice
{
	DECLARE_SINGLETON(CGraphicDevice)

	// �̱��� Ŭ�����̹Ƿ� ������ ���� �����ڸ� private���� ��.
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
	LPDIRECT3D9 m_pSDK;					// ��ġ�� ���� ���� ���� �� ��ġ�� ������ ��ü�� ������ com ��ü
	LPDIRECT3DDEVICE9 m_pDevice;		// ���������� ��ġ�� �ٷ� ��ü

	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
};

