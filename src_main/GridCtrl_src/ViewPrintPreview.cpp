// ViewPrintPreview.cpp : implementation file
//

#include "stdafx.h"
#include "ViewPrintPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview

IMPLEMENT_DYNCREATE(CViewPrintPreview, CView)

CViewPrintPreview::CViewPrintPreview()
{
	m_pOldFrame=NULL;
}

CViewPrintPreview::~CViewPrintPreview()
{
}


BEGIN_MESSAGE_MAP(CViewPrintPreview, CView)
	//{{AFX_MSG_MAP(CViewPrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview drawing

void CViewPrintPreview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview diagnostics

#ifdef _DEBUG
void CViewPrintPreview::AssertValid() const
{
	CView::AssertValid();
}

void CViewPrintPreview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPrintPreview message handlers

void CViewPrintPreview::OnFilePrintPreview()
{
	CView::OnFilePrintPreview();
}

BOOL CViewPrintPreview::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CViewPrintPreview::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pCtrl->OnBeginPrinting(pDC, pInfo);
}

void CViewPrintPreview::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pCtrl->OnPrint(pDC, pInfo);
}

void CViewPrintPreview::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_pCtrl->OnEndPrinting(pDC, pInfo);
}

void CViewPrintPreview::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// Show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// Restore main frame pointer
	AfxGetApp()->m_pMainWnd=m_pOldFrame;
	m_pCtrl->m_bPrintPreview=FALSE;
	// Kill parent frame and itself
	GetParentFrame()->DestroyWindow();
	//GetParentFrame()->ShowWindow(SW_HIDE); 

}
