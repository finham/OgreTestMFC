
// OgreMFCView.cpp : COgreMFCView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OgreMFC.h"
#endif

#include "OgreMFCDoc.h"
#include "OgreMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COgreMFCView

IMPLEMENT_DYNCREATE(COgreMFCView, CView)

BEGIN_MESSAGE_MAP(COgreMFCView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COgreMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// COgreMFCView 构造/析构

COgreMFCView::COgreMFCView()
{
	// TODO: 在此处添加构造代码
	m_firstDraw = true;
}

COgreMFCView::~COgreMFCView()
{
}

BOOL COgreMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COgreMFCView 绘制

void COgreMFCView::OnDraw(CDC* /*pDC*/)
{
	COgreMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//Ogre渲染
	try {
		if(m_firstDraw)
		{
			m_firstDraw = false;
			CRect   rect;
			GetClientRect(&rect);
			m_ogreApp.go(rect,m_hWnd);//传入MFC主窗口句柄

			SetTimer(1,20,0);
		}

		m_ogreApp.getRoot()->renderOneFrame();
	} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(  e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " <<
			e.getFullDescription().c_str() << std::endl;
#endif
	}
}


// COgreMFCView 打印


void COgreMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COgreMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COgreMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COgreMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void COgreMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COgreMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COgreMFCView 诊断

#ifdef _DEBUG
void COgreMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COgreMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COgreMFCDoc* COgreMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COgreMFCDoc)));
	return (COgreMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COgreMFCView 消息处理程序


void COgreMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent)
	{
		m_ogreApp.getRoot()->renderOneFrame();
	}
	CView::OnTimer(nIDEvent);
}


BOOL COgreMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}
