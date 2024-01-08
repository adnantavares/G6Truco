
// G6TrucoView.cpp : implementation of the CG6TrucoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "G6Truco.h"
#endif

#include "G6TrucoDoc.h"
#include "G6TrucoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CG6TrucoView

IMPLEMENT_DYNCREATE(CG6TrucoView, CView)

BEGIN_MESSAGE_MAP(CG6TrucoView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CG6TrucoView construction/destruction

CG6TrucoView::CG6TrucoView() noexcept
{
	// TODO: add construction code here
	backgroundBrush.CreateSolidBrush(RGB(0, 81, 44));


	memDCBack.CreateCompatibleDC(NULL);
	imageCardBack.Load(_T("res\\cards\\back_card.bmp"));
	bmpBack.Attach(imageCardBack.Detach());
	CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	memDCDeck.CreateCompatibleDC(NULL);
	imageDeck.Load(_T("res\\cards\\deck.png"));
	bmpDeck.Attach(imageDeck.Detach());
	CBitmap* pOldBmpDeck = memDCDeck.SelectObject(&bmpDeck);

	memDCCard1.CreateCompatibleDC(NULL);
	memDCCard2.CreateCompatibleDC(NULL);
	memDCCard3.CreateCompatibleDC(NULL);


}

CG6TrucoView::~CG6TrucoView()
{
}

BOOL CG6TrucoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CG6TrucoView drawing

void CG6TrucoView::OnDraw(CDC* pDC)
{
	CG6TrucoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, &backgroundBrush);

	// EXEMPLO DE COMO CARREGAR BITMAP DO RESOURCE
	//CBitmap bmpBack;
	//if (!bmpBack.LoadBitmap(IDB_BACK_CARD))
	//{
	//	// Falha ao carregar o bitmap
	//}
	//CDC memDCBack;
	//memDCBack.CreateCompatibleDC(NULL);
	//CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	
	imageCard1.Load(_T("res\\cards\\pj.png"));
	bmpCard1.Attach(imageCard1.Detach());
	CBitmap* pOldCard1 = memDCCard1.SelectObject(&bmpCard1);

	imageCard2.Load(_T("res\\cards\\pq.png"));
	bmpCard2.Attach(imageCard2.Detach());
	CBitmap* pOldCard2 = memDCCard2.SelectObject(&bmpCard2);

	imageCard3.Load(_T("res\\cards\\pk.png"));
	bmpCard3.Attach(imageCard3.Detach());
	CBitmap* pOldCard3 = memDCCard3.SelectObject(&bmpCard3);



	pDC->BitBlt(800, 0, 220, 320, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(860, 0, 220, 320, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(920, 0, 220, 320, &memDCCard1, 0, 0, SRCCOPY);

	pDC->BitBlt(0, 400, 220, 320, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(60, 400, 220, 320, &memDCCard1, 0, 0, SRCCOPY);
	pDC->BitBlt(120, 400, 220, 320, &memDCBack, 0, 0, SRCCOPY);

	pDC->BitBlt(800, 800, 220, 320, &memDCCard1, 0, 0, SRCCOPY);
	pDC->BitBlt(860, 800, 220, 320, &memDCCard2, 0, 0, SRCCOPY);
	if (cardClicked) {
		if (hideCard) {
			pDC->BitBlt(920, 770, 220, 320, &memDCBack, 0, 0, SRCCOPY);
		}
		else {
			pDC->BitBlt(920, 770, 220, 320, &memDCCard3, 0, 0, SRCCOPY);
		}
		m_ImageRect = CRect(920, 800, 920 + 220, 800 + 320);
	}
	else {
		pDC->BitBlt(920, 800, 220, 320, &memDCCard3, 0, 0, SRCCOPY);
		m_ImageRect = CRect(920, 780, 920 + 220, 780 + 320);
	}

	pDC->BitBlt(1600, 400, 220, 320, &memDCCard1, 0, 0, SRCCOPY);
	pDC->BitBlt(1660, 400, 220, 320, &memDCCard1, 0, 0, SRCCOPY);
	pDC->BitBlt(1720, 400, 220, 320, &memDCBack, 0, 0, SRCCOPY);

	pDC->BitBlt(860, 400, 220, 320, &memDCCard1, 0, 0, SRCCOPY);
	pDC->BitBlt(810, 500, 320, 220, &memDCDeck, 0, 0, SRCCOPY);

	bmpCard1.DeleteObject();
	bmpCard2.DeleteObject();
	bmpCard3.DeleteObject();
}

void CG6TrucoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_ImageRect.PtInRect(point))
	{
		cardClicked = !cardClicked;
		hideCard = false;
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CG6TrucoView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_ImageRect.PtInRect(point))
	{
		cardClicked = true;
		hideCard = true;
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

// CG6TrucoView diagnostics

#ifdef _DEBUG
void CG6TrucoView::AssertValid() const
{
	CView::AssertValid();
}

void CG6TrucoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CG6TrucoDoc* CG6TrucoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CG6TrucoDoc)));
	return (CG6TrucoDoc*)m_pDocument;
}
#endif //_DEBUG


// CG6TrucoView message handlers
