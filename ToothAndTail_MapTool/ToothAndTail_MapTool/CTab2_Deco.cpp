// CTab2_Deco.cpp: 구현 파일
//

#include "stdafx.h"
#include "ToothAndTail_MapTool.h"
#include "CTab2_Deco.h"
#include "afxdialogex.h"
#include "CTextureMgr.h"
#include "MainFrm.h"
#include "CForm.h"


// CTab2_Deco 대화 상자

IMPLEMENT_DYNAMIC(CTab2_Deco, CDialogEx)

CTab2_Deco::CTab2_Deco(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB2_DECO, pParent)
{

}

CTab2_Deco::~CTab2_Deco()
{
}

void CTab2_Deco::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DecoList);
}


BEGIN_MESSAGE_MAP(CTab2_Deco, CDialogEx)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CTab2_Deco::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CTab2_Deco 메시지 처리기


void CTab2_Deco::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	int iDropFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szFilePath[MAX_PATH] = L"";
	CString strRelativePath = L"";
	CString strFileNameAndExtension = L"";
	TCHAR szFileName[MAX_PATH] = L"";
	for (int i = 0; i < iDropFileCnt; ++i) {
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		strRelativePath = ConvertToRelativePath(szFilePath);
		strFileNameAndExtension = PathFindFileName(strRelativePath);
		lstrcpy(szFileName, strFileNameAndExtension.GetString());
		PathRemoveExtension(szFileName);
		m_DecoList.AddString(szFileName);
	}
	AdjustHorizontalScroll(m_DecoList);
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}

pair<wstring, int> CTab2_Deco::GetDecoType(void)
{
	return pair<wstring, int>(m_wstrDecoKey, m_iID);
}

void CTab2_Deco::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_wstrDecoKey = L"";
	m_iID = -1;

	CString strFileName = L"";
	int iSelectedIndex = m_DecoList.GetCurSel();

	m_DecoList.GetText(iSelectedIndex, strFileName);
	int i = 0;
	for (; i < strFileName.GetLength(); ++i)
	{
		if (isdigit(strFileName[i]))
			break;
	}
	//strFileName.Left
	//strFileName.Delete(0, i);
	//m_iDrawID = _ttoi(strFileName.GetString());
	CGraphicDevice::GetInstance()->BeginRender();

	wstring wstrDecoKey = strFileName.Left(i).GetString();
	int iID = _ttoi(strFileName.Right(strFileName.GetLength() - i).GetString());
	const TextureInfo* pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"Deco", wstrDecoKey, iID);
	if (nullptr == pTextureInfo)
		return;

	// 선택된 데코값 세팅
	m_wstrDecoKey = wstrDecoKey;
	m_iID = iID;

	float fCenterX = pTextureInfo->tImageInfo.Width >> 1;
	float fCenterY = pTextureInfo->tImageInfo.Height >> 1;

	D3DXMATRIX matScale, matTrans, matWorld;
	float fRatioX = WINCX / float(pTextureInfo->tImageInfo.Width);
	float fRatioY = WINCY / float(pTextureInfo->tImageInfo.Height);
	D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::GetInstance()->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	CGraphicDevice::GetInstance()->EndRender(pForm->m_PicView.GetSafeHwnd());

	UpdateData(FALSE);
}
