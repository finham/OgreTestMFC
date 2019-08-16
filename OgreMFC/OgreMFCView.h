
// OgreMFCView.h : COgreMFCView 类的接口
//

#pragma once
#include "MyOgreApp.h"

class COgreMFCView : public CView
{
protected: // 仅从序列化创建
	COgreMFCView();
	DECLARE_DYNCREATE(COgreMFCView)

// 特性
public:
	COgreMFCDoc* GetDocument() const;

// 操作
public:
	CMyOgreApp m_ogreApp;//ogre 主类
	bool m_firstDraw;//初次渲染

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COgreMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // OgreMFCView.cpp 中的调试版本
inline COgreMFCDoc* COgreMFCView::GetDocument() const
   { return reinterpret_cast<COgreMFCDoc*>(m_pDocument); }
#endif

