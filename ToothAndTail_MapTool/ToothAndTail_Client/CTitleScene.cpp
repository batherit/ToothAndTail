#include "stdafx.h"
#include "CTitleScene.h"
#include "CUI_Image.h"
#include "CUI_Button.h"
#include "CTextureMgr.h"
#include "CGameWorld.h"
#include "CSceneMgr.h"
#include "CPlayScene.h"
#include "CUI_FadeInOut.h"

CTitleScene::CTitleScene(CGameWorld & _rGameWorld)
	:
	CScene(_rGameWorld),
	m_pTitleImage(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"TITLE"), D3DXVECTOR3(WINCX >> 1, WINCY >> 1, 0.f))),
	m_pStartButtonImage(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON"), D3DXVECTOR3(250.f, (WINCY >> 1) + 100.f, 0.f))),
	m_pStartButtonOn(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON_ON"), D3DXVECTOR3(250.f-140.f, (WINCY >> 1) + 100.f, 0.f))),
	m_pStartButtonOff(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON_OFF"), D3DXVECTOR3(250.f - 140.f, (WINCY >> 1) + 100.f, 0.f))),
	m_pExitButtonImage(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON"), D3DXVECTOR3(250.f, (WINCY >> 1) + 250.f, 0.f))),
	m_pExitButtonOn(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON_ON"), D3DXVECTOR3(250.f - 140.f, (WINCY >> 1) + 250.f, 0.f))),
	m_pExitButtonOff(new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"BUTTON_OFF"), D3DXVECTOR3(250. - 140.f, (WINCY >> 1) + 250.f, 0.f)))
{
	m_pTitleImage->SetOutputArea(0, 0, WINCX, WINCY);
	
	m_pStartButton = new CUI_Button<CTitleScene>(_rGameWorld, 
		250.f, (WINCY >> 1) + 100.f,
		m_pStartButtonImage->GetWidth(), m_pStartButtonImage->GetHeight()
		, this, &CTitleScene::ClickStartButton);
	m_pStartButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_HOVERED, &CTitleScene::HoveredOnStartButton, nullptr);
	m_pStartButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_UNHOVERED, &CTitleScene::UnhoveredOnStartButton, nullptr);

	m_pExitButton = new CUI_Button<CTitleScene>(_rGameWorld,
		250.f, (WINCY >> 1) + 250.f,
		m_pExitButtonImage->GetWidth(), m_pStartButtonImage->GetHeight()
		, this, &CTitleScene::ClickQuitButton);
	m_pExitButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_HOVERED, &CTitleScene::HoveredOnExitButton, nullptr);
	m_pExitButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_UNHOVERED, &CTitleScene::UnhoveredOnExitButton, nullptr);
	CSoundMgr::GetInstance()->PlayBGM(L"Title.wav");

	m_pFadeInOutUI = new CUI_FadeInOut(_rGameWorld);
}

CTitleScene::~CTitleScene()
{
	Release();
}

void CTitleScene::ResetScene(void)
{
}

int CTitleScene::Update(float _fDeltaTime)
{
	m_pStartButton->Update(_fDeltaTime);
	m_pExitButton->Update(_fDeltaTime);
	if (1 == m_pFadeInOutUI->Update(_fDeltaTime)) {
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
		m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld));
	}
	return 0;
}

void CTitleScene::LateUpdate(void)
{
}

void CTitleScene::Render(CCamera * _pCamera)
{
	// 타이틀씬 배경화면 렌더
	m_pTitleImage->Render(nullptr);

	// 시작버튼 렌더
	m_pStartButtonImage->Render(nullptr);
	if (m_bIsOnStartButtonHovered) m_pStartButtonOn->Render(nullptr);
	else m_pStartButtonOff->Render(nullptr);
	TCHAR szBuf[64] = L"";
	D3DXVECTOR3 vTextPos = m_pStartButton->GetXY();
	vTextPos += D3DXVECTOR3(-115.f, -40.f, 0.f);
	CGraphicDevice::GetInstance()->RenderText(L"게임 시작", vTextPos, 0.9f);

	// 종료버튼 렌더
	m_pExitButtonImage->Render(nullptr);
	if (m_bIsOnExitButtonHovered) m_pExitButtonOn->Render(nullptr);
	else m_pExitButtonOff->Render(nullptr);
	vTextPos = m_pExitButton->GetXY();
	vTextPos += D3DXVECTOR3(-115.f, -40.f, 0.f);
	CGraphicDevice::GetInstance()->RenderText(L"게임 종료", vTextPos, 0.9f);

	CGraphicDevice::GetInstance()->RenderText(L"110C 심정환", D3DXVECTOR3(WINCX - 250.f, WINCY - 50.f, 0.f), 0.6f);

	m_pFadeInOutUI->Render(nullptr);
}

void CTitleScene::Release(void)
{
	SafelyDeleteObj(m_pTitleImage);
	SafelyDeleteObj(m_pStartButtonOn);
	SafelyDeleteObj(m_pStartButtonOff);
	SafelyDeleteObj(m_pStartButtonImage);
	SafelyDeleteObj(m_pStartButton);
	SafelyDeleteObj(m_pExitButtonOn);
	SafelyDeleteObj(m_pExitButtonOff);
	SafelyDeleteObj(m_pExitButtonImage);
	SafelyDeleteObj(m_pExitButton);
	SafelyDeleteObj(m_pFadeInOutUI);
}

LRESULT CTitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void CTitleScene::HoveredOnStartButton(void *)
{
	// 시작 버튼 온
	if (!m_bIsOnStartButtonHovered) {
		CSoundMgr::GetInstance()->PlaySound(TEXT("Menu_OnMouse.wav"), CSoundMgr::UI);
		m_bIsOnStartButtonHovered = true;
	}
}

void CTitleScene::UnhoveredOnStartButton(void *)
{
	// 시작 버튼 오프
	m_bIsOnStartButtonHovered = false;
}

void CTitleScene::ClickStartButton(void *)
{
	// TODO : 플레이 씬으로 넘어간다.
	//CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
	//m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld));
	m_pFadeInOutUI->StartFadeInOut(1.2f, false);
	CSoundMgr::GetInstance()->PlaySound(L"Menu_Select.wav", CSoundMgr::UI);
}

void CTitleScene::HoveredOnExitButton(void *)
{
	// 종료 버튼 온
	if (!m_bIsOnExitButtonHovered) {
		CSoundMgr::GetInstance()->PlaySound(TEXT("Menu_OnMouse.wav"), CSoundMgr::UI);
		m_bIsOnExitButtonHovered = true;
	}
}

void CTitleScene::UnhoveredOnExitButton(void *)
{
	// 종료 버튼 오프
	m_bIsOnExitButtonHovered = false;
}

void CTitleScene::ClickQuitButton(void *)
{
	DestroyWindow(g_hWND);
}
