
// G6TrucoView.cpp : implementation of the CG6TrucoView class
//

#include "pch.h"
#include "framework.h"
#include <vector>
#include <algorithm>

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
	ON_BN_CLICKED(BUTTONTRUCOID, OnButtonTrucoClicked)
	ON_BN_CLICKED(BUTTONNEWGAMEID, OnButtonNewGameClicked)
	ON_BN_CLICKED(BUTTONPLAYCARDID, OnButtonPlayCardClicked)
END_MESSAGE_MAP()

// CG6TrucoView construction/destruction

CG6TrucoView::CG6TrucoView() noexcept
{
	// TODO: add construction code here
	backgroundBrush.CreateSolidBrush(RGB(0, 81, 44));


	memDCBack.CreateCompatibleDC(NULL);
	imageCardBack.Load(_T("res\\cards\\back.png"));
	bmpBack.Attach(imageCardBack.Detach());
	CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	memDCDeck.CreateCompatibleDC(NULL);
	imageDeck.Load(_T("res\\cards\\deck.png"));
	bmpDeck.Attach(imageDeck.Detach());
	CBitmap* pOldBmpDeck = memDCDeck.SelectObject(&bmpDeck);

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

#pragma region Setting events
	controller.ActivePlayerChangedEventListener(std::bind(&CG6TrucoView::OnActivePlayerChangedEvent, this, std::placeholders::_1));
	controller.RoundInformationChangedEventListener(std::bind(&CG6TrucoView::OnRoundInformationsChangedEvent, this, std::placeholders::_1));
#pragma endregion

}

CG6TrucoView::~CG6TrucoView()
{
}

//Use the following events to update the view informations
#pragma region Events

void CG6TrucoView::OnActivePlayerChangedEvent(Player* player) {
	//player->GetPlayerName();
	//player->GetHand();
}

void CG6TrucoView::OnRoundInformationsChangedEvent(Round* currentRoundInformations) {
	//currentRoundInformations->GetViraCard();
	//currentRoundInformations->GetAllPlayers();
	//currentRoundInformations->GetActivePlayer();
	//currentRoundInformations->GetActivePlayer()->GetPlayerName();
	currentRound = currentRoundInformations;
	//currentPlayers = RotatePlayers(currentRound->GetAllPlayers(), currentRound->GetActivePlayerIndex());
	Invalidate();
}
#pragma endregion


//TODO: Replace this to MVC pattern
void CG6TrucoView::OnBnClickedRaiseBet()
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

	if (start) {
		DrawCards(pDC);
	}

	pDC->BitBlt(820, 400, cardH, cardW, &memDCDeck, 0, 0, SRCCOPY);

	UpdateButtons();
}

void CG6TrucoView::DrawPlayerCards(CDC* pDC, Player* p, int x, int y) {
	for (int c = 0; c < p->GetHand().size(); c++) {
		if (c == p->GetSelectCardIndex()) {
			pDC->BitBlt(x, y, cardW, cardH, cardsMap[p->GetHand().at(c).GetSuit()][p->GetHand().at(c).GetRank()], 0, 0, SRCCOPY);
		}
		else {
			pDC->BitBlt(x, y, cardW, cardH, &memDCBack, 0, 0, SRCCOPY);
		}
		x += 60;
	}
}

void CG6TrucoView::DrawCards(CDC* pDC) {
    std::vector<Player*> players;
	for (int a = 0; a < 4; a++) {
		players.push_back(currentRound->GetAllPlayers().at(a).get());
	}
	int n = currentRound->GetActivePlayerIndex();
	std::rotate(players.begin(), players.begin() + n, players.end());

	pDC->BitBlt(860, 320, cardW, cardH, cardsMap[currentRound->GetViraCard().GetSuit()][currentRound->GetViraCard().GetRank()], 0, 0, SRCCOPY);

	DrawPlayerCards(pDC, players.at(1), 40, 350);
	pDC->TextOut(140, 600, players.at(1)->GetPlayerName());

	DrawPlayerCards(pDC, players.at(2), 800, 10);
	pDC->TextOut(880, 260, players.at(2)->GetPlayerName());

	DrawPlayerCards(pDC, players.at(3), 1600, 350);
	pDC->TextOut(1700, 600, players.at(3)->GetPlayerName());

	pDC->TextOut(880, 900, players.at(0)->GetPlayerName());
	if (players.at(0)->GetHand().size() >= 1) {
		if (cardClicked == 1) {
			if (hideCard) {
				pDC->BitBlt(800, 620, cardW, cardH, &memDCBack, 0, 0, SRCCOPY);
			}
			else {
				pDC->BitBlt(800, 620, cardW, cardH, cardsMap[players.at(0)->GetHand().at(0).GetSuit()][players.at(0)->GetHand().at(0).GetRank()], 0, 0, SRCCOPY);
			}
			m_Card1Rect = CRect(800, 620, 800 + cardW, 620 + cardH);
		}
		else {
			pDC->BitBlt(800, 650, cardW, cardH, cardsMap[players.at(0)->GetHand().at(0).GetSuit()][players.at(0)->GetHand().at(0).GetRank()], 0, 0, SRCCOPY);
			m_Card1Rect = CRect(800, 650, 800 + cardW, 650 + cardH);
		}
		if (players.at(0)->GetHand().size() >= 2) {
			if (cardClicked == 2) {
				if (hideCard) {
					pDC->BitBlt(860, 620, cardW, cardH, &memDCBack, 0, 0, SRCCOPY);
				}
				else {
					pDC->BitBlt(860, 620, cardW, cardH, cardsMap[players.at(0)->GetHand().at(1).GetSuit()][players.at(0)->GetHand().at(1).GetRank()], 0, 0, SRCCOPY);
				}
				m_Card2Rect = CRect(860, 620, 860 + cardW, 620 + cardH);
			}
			else {
				pDC->BitBlt(860, 650, cardW, cardH, cardsMap[players.at(0)->GetHand().at(1).GetSuit()][players.at(0)->GetHand().at(1).GetRank()], 0, 0, SRCCOPY);
				m_Card2Rect = CRect(860, 650, 860 + cardW, 650 + cardH);
			}
			if (players.at(0)->GetHand().size() >= 3) {
				if (cardClicked == 3) {
					if (hideCard) {
						pDC->BitBlt(920, 620, cardW, cardH, &memDCBack, 0, 0, SRCCOPY);
					}
					else {
						pDC->BitBlt(920, 620, cardW, cardH, cardsMap[players.at(0)->GetHand().at(2).GetSuit()][players.at(0)->GetHand().at(2).GetRank()], 0, 0, SRCCOPY);
					}
					m_Card3Rect = CRect(920, 620, 920 + cardW, 620 + cardH);
				}
				else {
					pDC->BitBlt(920, 650, cardW, cardH, cardsMap[players.at(0)->GetHand().at(2).GetSuit()][players.at(0)->GetHand().at(2).GetRank()], 0, 0, SRCCOPY);
					m_Card3Rect = CRect(920, 650, 920 + cardW, 650 + cardH);
				}
			}
		}
	}
	
}

void CG6TrucoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!currentRound->IsHumanPlayer()) {
		return;
	}
	Player* p = currentRound->GetActivePlayer();
	if (m_Card3Rect.PtInRect(point) && p->GetHand().size() >= 3)
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
	else if (m_Card2Rect.PtInRect(point) && p->GetHand().size() >= 2) {
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
	else if (m_Card1Rect.PtInRect(point) && p->GetHand().size() >= 1) {
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

void CG6TrucoView::TryPlayCard(int cardIndex)
{
	bool cardHasBeenSelected = controller.TrySetSelectedCardIndex(cardIndex);
	if (cardHasBeenSelected) controller.PlayCard();
}

void CG6TrucoView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!currentRound->IsHumanPlayer()) {
		return;
	}
	Player* p = currentRound->GetActivePlayer();
	if (m_Card3Rect.PtInRect(point) && p->GetHand().size() >= 3)
	{
		cardClicked = 3;
		hideCard = true;
		Invalidate();
	}
	else if (m_Card2Rect.PtInRect(point) && p->GetHand().size() >= 2) {
		cardClicked = 2;
		hideCard = true;
		Invalidate();
	}
	else if (m_Card1Rect.PtInRect(point) && p->GetHand().size() >= 1) {
		cardClicked = 1;
		hideCard = true;
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CG6TrucoView::OnInitialUpdate()
{
	//Create Button
	buttonNewGame.Create(L"New Game", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, BUTTONNEWGAMEID);
	buttonNewGame.MoveWindow(850, 650, 180, 80);

	CreateButton(buttonPlayCard, _T("Play Card"), CRect(1100, 750, 1250, 810), BUTTONPLAYCARDID);
	CreateButton(buttonAcceptTruco, _T("Accept"), CRect(100, 100, 200, 140), BUTTONACCEPTTRUCOID);
	CreateButton(buttonTruco, _T("Truco"), CRect(210, 100, 310, 140), BUTTONTRUCOID);
	CreateButton(buttonRejectTruco, _T("Reject"), CRect(320, 100, 420, 140), BUTTONREJECTTRUCOID);
}

void CG6TrucoView::CreateButton(CButton& button, LPCTSTR contentText, CRect rectButton, int idButton)
{
	if (!button.Create(contentText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectButton, this, idButton))
	{
		AfxMessageBox(_T("Failed to create button!"));
	}
}

void CG6TrucoView::UpdateButtons() {
	if (start) {
		buttonNewGame.ShowWindow(SW_HIDE);
		if (currentRound->IsHumanPlayer()) {
			buttonTruco.ShowWindow(SW_SHOW);
			buttonAcceptTruco.ShowWindow(SW_SHOW);
			buttonRejectTruco.ShowWindow(SW_SHOW);
			buttonPlayCard.ShowWindow(SW_SHOW);
		}
		else {
			buttonTruco.ShowWindow(SW_HIDE);
			buttonAcceptTruco.ShowWindow(SW_HIDE);
			buttonRejectTruco.ShowWindow(SW_HIDE);
			buttonPlayCard.ShowWindow(SW_HIDE);
		}
	}
	else {
		buttonNewGame.ShowWindow(SW_SHOW);
		buttonTruco.ShowWindow(SW_HIDE);
		buttonAcceptTruco.ShowWindow(SW_HIDE);
		buttonRejectTruco.ShowWindow(SW_HIDE);
		buttonPlayCard.ShowWindow(SW_HIDE);
	}
}

void CG6TrucoView::OnButtonTrucoClicked() 
{
	controller.RaiseBet();
	buttonTruco.SetWindowTextW(L"TESTE");
	SetStatusBarText(L"Truco Button Clicked");
}

void CG6TrucoView::OnButtonNewGameClicked()
{
	SetStatusBarText(L"New Game Button Clicked");
	start = true;
	controller.StartGame();
}

void CG6TrucoView::OnButtonPlayCardClicked()
{
	if (cardClicked >= 1) {
		TryPlayCard(cardClicked - 1);
		cardClicked = 0;
	}
	else {
		SetStatusBarText(L"No Card Selected");
	}
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
