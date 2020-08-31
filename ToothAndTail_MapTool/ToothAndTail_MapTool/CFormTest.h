﻿#pragma once



// CFormTest 폼 보기

class CFormTest : public CFormView
{
	DECLARE_DYNCREATE(CFormTest)

protected:
	CFormTest();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTest();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMTEST };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


