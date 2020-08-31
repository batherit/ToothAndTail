#include "stdafx.h"
#include "CMapEditor.h"
#include "CCamera.h"
#include "CTextureMgr.h"
#include "CSpriteObj.h"


CMapEditor::CMapEditor()
{
}


CMapEditor::~CMapEditor()
{
	Release();
}

//LRESULT CMapEditor::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	return LRESULT();
//}

void CMapEditor::Ready(void)
{
	LoadTextures();

	m_pMap = new CSpriteObj(*this, (MAP_WIDTH >> 1), (MAP_HEIGHT >> 1), MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pCamera = new CCamera(*this, nullptr, 0.f, 0.f);
	SetMainCamera(m_pCamera);		//.. 메인 카메라 세팅
}

void CMapEditor::Update(void)
{
}

void CMapEditor::LateUpdate(void)
{
}

void CMapEditor::Render(void)
{
	StartRender();

	// TODO : 그리기 연산을 수행합니다.
	m_pMap->Render(GetMainCamera());

	EndRender();
}

void CMapEditor::Release(void)
{
	SafelyDeleteObj(m_pMap);
	SafelyDeleteObj(m_pCamera);
}

void CMapEditor::LoadTextures(void)
{
	// 베이스 맵 텍스쳐 생성
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP")))
		return;
}
