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
	// 장치 초기화 과정
	// 1. SDK(com) 생성
	// 2. SDK를 통해 장치 지원 수준 조사
	// 3. 지원 수준에 맞는 디바이스(com, 장치 제어 객체)를 생성.

	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	//1.장치의 수준을 조사할 컴객체를 생성. 
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// 실패 했을때에 대한 메시지를 넣어줄 것. 
		ERR_MSG(L"Failed to get the device caps.");

		return E_FAIL;
	}

	// DeviceCaps로부터 정점/조명 연산이 가능한 장치인지 조사하고,
	// 연산 모드를 세팅한다.
	DWORD vp = 0;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 출력 정보를 세팅한다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WINCX;					// 백버퍼 가로 길이
	d3dpp.BackBufferHeight = WINCY;					// 백버퍼 세로 길이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		// 백버퍼 원소 포멧
	d3dpp.BackBufferCount = 1;						// 백버퍼 갯수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	// 백버퍼 샘플링 모드
	d3dpp.MultiSampleQuality = 0;					// 백버퍼 샘플링 수준


	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 백버퍼 교체 방식
	d3dpp.hDeviceWindow = g_hWND;					// 메인 윈도우 세팅
	d3dpp.Windowed = TRUE;							// 윈도우창 모드인지? => MFC는 전체화면 지원 x
	d3dpp.EnableAutoDepthStencil = TRUE;			// 뎁스스텐실을 사용할 것인지?
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// 뎁스스텐실 원소 포멧
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// 전체화면시 리프레쉬 주사율
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// Refresh Rate와 장면의 Present 처리의 관계를 설정한다.

	// 장치를 생성한다.
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Failed to create the hareware device.");
		return E_FAIL;
	}

	// 스프라이트를 세팅한다.
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Failed to create the sprite.");
		return E_FAIL;
	}

	// 폰트 정보를 세팅한다.
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	//tFontInfo.Height = DEFAULT_;
	//tFontInfo.Width = 24;
	tFontInfo.Height = 70;
	tFontInfo.Width = 35;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = DEFAULT_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"FFF Forward");

	// 폰트를 생성한다.
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
