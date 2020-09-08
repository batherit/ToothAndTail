#include "stdafx.h"
#include "CTitleScene.h"
#include "CUI_Image.h"
#include "CUI_Button.h"
#include "CTextureMgr.h"
#include "CGameWorld.h"
#include "CSceneMgr.h"
#include "CTestScene.h"

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

	return 0;
}

void CTitleScene::LateUpdate(void)
{
}

void CTitleScene::Render(CCamera * _pCamera)
{
	// Ÿ��Ʋ�� ���ȭ�� ����
	m_pTitleImage->Render(nullptr);

	// ���۹�ư ����
	m_pStartButtonImage->Render(nullptr);
	if (m_bIsOnStartButtonHovered) m_pStartButtonOn->Render(nullptr);
	else m_pStartButtonOff->Render(nullptr);
	TCHAR szBuf[64] = L"";
	D3DXVECTOR3 vTextPos = m_pStartButton->GetXY();
	vTextPos += D3DXVECTOR3(-115.f, -40.f, 0.f);
	CGraphicDevice::GetInstance()->RenderText(L"���� ����", vTextPos, 0.9f);

	// �����ư ����
	m_pExitButtonImage->Render(nullptr);
	if (m_bIsOnExitButtonHovered) m_pExitButtonOn->Render(nullptr);
	else m_pExitButtonOff->Render(nullptr);
	vTextPos = m_pExitButton->GetXY();
	vTextPos += D3DXVECTOR3(-115.f, -40.f, 0.f);
	CGraphicDevice::GetInstance()->RenderText(L"���� ����", vTextPos, 0.9f);
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
}

void CTitleScene::HoveredOnStartButton(void *)
{
	// ���� ��ư ��
	m_bIsOnStartButtonHovered = true;
}

void CTitleScene::UnhoveredOnStartButton(void *)
{
	// ���� ��ư ����
	m_bIsOnStartButtonHovered = false;
}

void CTitleScene::ClickStartButton(void *)
{
	// TODO : �÷��� ������ �Ѿ��.
	m_rGameWorld.GetSceneManager()->SetNextScene(new CTestScene(m_rGameWorld));
}

void CTitleScene::HoveredOnExitButton(void *)
{
	// ���� ��ư ��
	m_bIsOnExitButtonHovered = true;
}

void CTitleScene::UnhoveredOnExitButton(void *)
{
	// ���� ��ư ����
	m_bIsOnExitButtonHovered = false;
}

void CTitleScene::ClickQuitButton(void *)
{
	DestroyWindow(g_hWND);
}