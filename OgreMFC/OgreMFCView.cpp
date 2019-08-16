
// OgreMFCView.cpp : COgreMFCView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COgreMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// COgreMFCView ����/����

COgreMFCView::COgreMFCView()
{
	// TODO: �ڴ˴���ӹ������
	m_firstDraw = true;
}

COgreMFCView::~COgreMFCView()
{
}

BOOL COgreMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COgreMFCView ����

void COgreMFCView::OnDraw(CDC* /*pDC*/)
{
	COgreMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//Ogre��Ⱦ
	try {
		if(m_firstDraw)
		{
			m_firstDraw = false;
			CRect   rect;
			GetClientRect(&rect);
			m_ogreApp.go(rect,m_hWnd);//����MFC�����ھ��

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


// COgreMFCView ��ӡ


void COgreMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COgreMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void COgreMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void COgreMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// COgreMFCView ���

#ifdef _DEBUG
void COgreMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COgreMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COgreMFCDoc* COgreMFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COgreMFCDoc)));
	return (COgreMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COgreMFCView ��Ϣ�������


void COgreMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1 == nIDEvent)
	{
		m_ogreApp.getRoot()->renderOneFrame();
	}
	CView::OnTimer(nIDEvent);
}


BOOL COgreMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}
