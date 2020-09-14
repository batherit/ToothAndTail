#include "stdafx.h"
#include "CGraphicDevice.h"

INITIALIZE_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{
}


CGraphicDevice::~CGraphicDevice()
{
	ReleaseGraphicDevice();
}

void CGraphicDevice::RenderText(const wstring & _wstrText, D3DXVECTOR3 _vPos, float _fScale, D3DXCOLOR _clRenderColor)
{
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixScaling(&matScale, _fScale, _fScale, _fScale);
	D3DXMatrixTranslation(&matTrans, _vPos.x, _vPos.y, _vPos.z);
	matWorld = matScale * matTrans;
	m_pSprite->SetTransform(&matWorld);

	m_pFont->DrawTextW(m_pSprite, _wstrText.c_str(), _wstrText.length(), nullptr, 0, _clRenderColor);
}

HRESULT CGraphicDevice::GenerateGraphicDevice()
{
	// ��ġ �ʱ�ȭ ����
	// 1. SDK(com) ����
	// 2. SDK�� ���� ��ġ ���� ���� ����
	// 3. ���� ���ؿ� �´� ����̽�(com, ��ġ ���� ��ü)�� ����.

	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	//1.��ġ�� ������ ������ �İ�ü�� ����. 
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// ���� �������� ���� �޽����� �־��� ��. 
		ERR_MSG(L"Failed to get the device caps.");

		return E_FAIL;
	}

	// DeviceCaps�κ��� ����/���� ������ ������ ��ġ���� �����ϰ�,
	// ���� ��带 �����Ѵ�.
	DWORD vp = 0;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// ��� ������ �����Ѵ�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WINCX;					// ����� ���� ����
	d3dpp.BackBufferHeight = WINCY;					// ����� ���� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		// ����� ���� ����
	d3dpp.BackBufferCount = 1;						// ����� ����

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	// ����� ���ø� ���
	d3dpp.MultiSampleQuality = 0;					// ����� ���ø� ����


	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// ����� ��ü ���
	d3dpp.hDeviceWindow = g_hWND;					// ���� ������ ����
	d3dpp.Windowed = TRUE;							// ������â �������? => MFC�� ��üȭ�� ���� x
	d3dpp.EnableAutoDepthStencil = TRUE;			// �������ٽ��� ����� ������?
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// �������ٽ� ���� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ��üȭ��� �������� �ֻ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// Refresh Rate�� ����� Present ó���� ���踦 �����Ѵ�.

	// ��ġ�� �����Ѵ�.
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Failed to create the hareware device.");
		return E_FAIL;
	}

	// ��������Ʈ�� �����Ѵ�.
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Failed to create the sprite.");
		return E_FAIL;
	}

	// ��Ʈ ������ �����Ѵ�.
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	//tFontInfo.Height = DEFAULT_;
	//tFontInfo.Width = 24;
	tFontInfo.Height = 70;
	tFontInfo.Width = 35;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = DEFAULT_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"FFF Forward");

	// ��Ʈ�� �����Ѵ�.
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Failed to create the font.");
		return E_FAIL;
	}



	return S_OK;
}

void CGraphicDevice::ReleaseGraphicDevice()
{
	SafelyReleaseCOM(m_pFont);
	SafelyReleaseCOM(m_pSprite);
	SafelyReleaseCOM(m_pDevice);
	SafelyReleaseCOM(m_pSDK);
}

void CGraphicDevice::BeginRender(void)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 0, 0, 0), 0.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphicDevice::EndRender(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
