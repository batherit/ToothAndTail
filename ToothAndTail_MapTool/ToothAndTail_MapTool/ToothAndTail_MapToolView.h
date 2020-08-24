﻿
// ToothAndTail_MapToolView.h: CToothAndTailMapToolView 클래스의 인터페이스
//

#pragma once


class CToothAndTailMapToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToothAndTailMapToolView() noexcept;
	DECLARE_DYNCREATE(CToothAndTailMapToolView)

// 특성입니다.
public:
	CToothAndTailMapToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToothAndTailMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // ToothAndTail_MapToolView.cpp의 디버그 버전
inline CToothAndTailMapToolDoc* CToothAndTailMapToolView::GetDocument() const
   { return reinterpret_cast<CToothAndTailMapToolDoc*>(m_pDocument); }
#endif
