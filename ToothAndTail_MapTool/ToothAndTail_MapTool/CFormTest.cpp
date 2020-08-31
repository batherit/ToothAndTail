// CFormTest.cpp: 구현 파일
//

#include "pch.h"
#include "ToothAndTail_MapTool.h"
#include "CFormTest.h"


// CFormTest

IMPLEMENT_DYNCREATE(CFormTest, CFormView)

CFormTest::CFormTest()
	: CFormView(IDD_FORMTEST)
{

}

CFormTest::~CFormTest()
{
}

void CFormTest::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTest, CFormView)
END_MESSAGE_MAP()


// CFormTest 진단

#ifdef _DEBUG
void CFormTest::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTest::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTest 메시지 처리기
