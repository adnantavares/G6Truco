
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
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CG6TrucoView

IMPLEMENT_DYNCREATE(CG6TrucoView, CView)

BEGIN_MESSAGE_MAP(CG6TrucoView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(BUTTON1ID, OnButton1Clicked)
END_MESSAGE_MAP()

// CG6TrucoView construction/destruction

CG6TrucoView::CG6TrucoView() noexcept
{
	// TODO: add construction code here
	backgroundBrush.CreateSolidBrush(RGB(0, 81, 44));


	memDCBack.CreateCompatibleDC(NULL);
	imageCardBack.Load(_T("res\\cards\\back.bmp"));
	bmpBack.Attach(imageCardBack.Detach());
	CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	memDCDeck.CreateCompatibleDC(NULL);
	imageDeck.Load(_T("res\\cards\\deck.png"));
	bmpDeck.Attach(imageDeck.Detach());
	CBitmap* pOldBmpDeck = memDCDeck.SelectObject(&bmpDeck);

	//memDCCard1.CreateCompatibleDC(NULL);
	//memDCCard2.CreateCompatibleDC(NULL);
	//memDCCard3.CreateCompatibleDC(NULL);
	
	//Initialize Image Matrix will all Deck
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 10; b++) {
			cardsMap[a][b] = new CDC;
			cardsMap[a][b]->CreateCompatibleDC(NULL);
			CImage imageCard;
			std::string pathCard = "res\\cards\\" + cardsNameMap[a][b] + ".png";
			std::wstring wide_string(pathCard.begin(), pathCard.end());
			imageCard.Load(wide_string.c_str());
			CBitmap bmpCard;
			bmpCard.Attach(imageCard.Detach());
			CBitmap* pOldBmpDeck = cardsMap[a][b]->SelectObject(&bmpCard);
		}
	}


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

	// How to load a bitmap from resources
	//CBitmap bmpBack;
	//if (!bmpBack.LoadBitmap(IDB_BACK_CARD))
	//{
	//	// Falha ao carregar o bitmap
	//}
	//CDC memDCBack;
	//memDCBack.CreateCompatibleDC(NULL);
	//CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	//std::string pathCard = "res\\cards\\" + cardsNameMap[2][3] + ".png";
	//std::wstring wide_string(pathCard.begin(), pathCard.end());
	//imageCard1.Load(wide_string.c_str());
	//bmpCard1.Attach(imageCard1.Detach());
	//CBitmap* pOldCard1 = memDCCard1.SelectObject(&bmpCard1);

	//imageCard2.Load(_T("res\\cards\\pq.png"));
	//bmpCard2.Attach(imageCard2.Detach());
	//CBitmap* pOldCard2 = memDCCard2.SelectObject(&bmpCard2);

	//imageCard3.Load(_T("res\\cards\\pk.png"));
	//bmpCard3.Attach(imageCard3.Detach());
	//CBitmap* pOldCard3 = memDCCard3.SelectObject(&bmpCard3);



	pDC->BitBlt(800, 0, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(860, 0, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(920, 0, cardW,  cardH, cardsMap[2][6], 0, 0, SRCCOPY);

	pDC->BitBlt(0, 400, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
	pDC->BitBlt(60, 400, cardW,  cardH, cardsMap[0][5], 0, 0, SRCCOPY);
	pDC->BitBlt(120, 400, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);


	if (cardClicked == 1) {
		if (hideCard) {
			pDC->BitBlt(800, 770, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
		}
		else {
			pDC->BitBlt(800, 770, cardW,  cardH, cardsMap[0][5], 0, 0, SRCCOPY);
		}
		m_Card1Rect = CRect(800, 770, 800 + cardW, 770 +  cardH);
	}
	else {
		pDC->BitBlt(800, 800, cardW,  cardH, cardsMap[0][5], 0, 0, SRCCOPY);
		m_Card1Rect = CRect(800, 800, 800 + cardW, 800 +  cardH);
	}
	
	if (cardClicked == 2) {
		if (hideCard) {
			pDC->BitBlt(860, 770, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
		}
		else {
			pDC->BitBlt(860, 770, cardW,  cardH, cardsMap[1][5], 0, 0, SRCCOPY);
		}
		m_Card2Rect = CRect(860, 770, 860 + cardW, 770 +  cardH);
	}
	else {
		pDC->BitBlt(860, 800, cardW,  cardH, cardsMap[1][5], 0, 0, SRCCOPY);
		m_Card2Rect = CRect(860, 800, 860 + cardW, 800 +  cardH);
	}

	if (cardClicked == 3) {
		if (hideCard) {
			pDC->BitBlt(920, 770, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);
		}
		else {
			pDC->BitBlt(920, 770, cardW,  cardH, cardsMap[3][5], 0, 0, SRCCOPY);
		}
		m_Card3Rect = CRect(920, 770, 920 + cardW, 770 +  cardH);
	}
	else {
		pDC->BitBlt(920, 800, cardW,  cardH, cardsMap[3][5], 0, 0, SRCCOPY);
		m_Card3Rect = CRect(920, 800, 920 + cardW, 800 +  cardH);
	}
	
	pDC->BitBlt(1600, 400, cardW,  cardH, cardsMap[1][2], 0, 0, SRCCOPY);
	pDC->BitBlt(1660, 400, cardW,  cardH, cardsMap[1][5], 0, 0, SRCCOPY);
	pDC->BitBlt(1720, 400, cardW,  cardH, &memDCBack, 0, 0, SRCCOPY);

	pDC->BitBlt(860, 400, cardW,  cardH, cardsMap[3][4], 0, 0, SRCCOPY);
	pDC->BitBlt(810, 500,  cardH, cardW, &memDCDeck, 0, 0, SRCCOPY);

	//bmpCard1.DeleteObject();
	//bmpCard2.DeleteObject();
	//bmpCard3.DeleteObject();
}

void CG6TrucoView::DrawCards(CDC* pDC) {


}

void CG6TrucoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Card3Rect.PtInRect(point))
	{
		if (cardClicked == 3) {
			cardClicked = 0;
		}
		else {
			cardClicked = 3;
		}
		hideCard = false;
		SetStatusBarText(L"Card 3 Clicked");
		Invalidate();
	}
	else if (m_Card2Rect.PtInRect(point)) {
		if (cardClicked ==2) {
			cardClicked = 0;
		}
		else {
			cardClicked = 2;
		}
		hideCard = false;
		SetStatusBarText(L"Card 2 Clicked");
		Invalidate();
	}
	else if (m_Card1Rect.PtInRect(point)) {
		if (cardClicked == 1) {
			cardClicked = 0;
		}
		else {
			cardClicked = 1;
		}
		hideCard = false;
		SetStatusBarText(L"Card 1 Clicked");
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CG6TrucoView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_Card3Rect.PtInRect(point))
	{
		cardClicked = 3;
		hideCard = true;
		Invalidate();
	}
	else if (m_Card2Rect.PtInRect(point)) {
		cardClicked = 2;
		hideCard = true;
		Invalidate();
	}
	else if (m_Card1Rect.PtInRect(point)) {
		cardClicked = 1;
		hideCard = true;
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CG6TrucoView::OnInitialUpdate()
{
	//Create Button
	m_Button.Create(L"Truco", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, BUTTON1ID);
	RepositionButton();
}

void CG6TrucoView::RepositionButton()
{
	//Button position
	m_Button.MoveWindow(1300, 820, 180, 80);
}

void CG6TrucoView::OnButton1Clicked() 
{
	SetStatusBarText(L"Truco Button Clicked");
}

void CG6TrucoView::SetStatusBarText(const CString& strText) {
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrm != nullptr)
	{
		pMainFrm->SetStatusBarText(strText);
	}
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
