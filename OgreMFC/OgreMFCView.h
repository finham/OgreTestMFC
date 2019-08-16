
// OgreMFCView.h : COgreMFCView ��Ľӿ�
//

#pragma once
#include "MyOgreApp.h"

class COgreMFCView : public CView
{
protected: // �������л�����
	COgreMFCView();
	DECLARE_DYNCREATE(COgreMFCView)

// ����
public:
	COgreMFCDoc* GetDocument() const;

// ����
public:
	CMyOgreApp m_ogreApp;//ogre ����
	bool m_firstDraw;//������Ⱦ

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~COgreMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // OgreMFCView.cpp �еĵ��԰汾
inline COgreMFCDoc* COgreMFCView::GetDocument() const
   { return reinterpret_cast<COgreMFCDoc*>(m_pDocument); }
#endif

