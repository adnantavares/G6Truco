
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
	ON_BN_CLICKED(BUTTONACCEPTTRUCOID, OnButtonAcceptTrucoClicked)
	ON_BN_CLICKED(BUTTONREJECTTRUCOID, OnButtonRejectTrucoClicked)
END_MESSAGE_MAP()

// CG6TrucoView construction/destruction

CG6TrucoView::CG6TrucoView() noexcept
{
	backgroundBrush.CreateSolidBrush(RGB(0, 81, 44));

	memDCBack.CreateCompatibleDC(NULL);
	imageCardBack.Load(_T("res\\cards\\back.png"));
	bmpBack.Attach(imageCardBack.Detach());
	CBitmap* pOldBack = memDCBack.SelectObject(&bmpBack);

	memDCDeck.CreateCompatibleDC(NULL);
	imageDeck.Load(_T("res\\cards\\deck.png"));
	bmpDeck.Attach(imageDeck.Detach());
	CBitmap* pOldBmpDeck = memDCDeck.SelectObject(&bmpDeck);

	//Initialize Image Matrix with all Deck cards in order to speed up OnDraw process
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
}

void CG6TrucoView::OnRoundInformationsChangedEvent(Round* currentRoundInformations) {
	currentRound = currentRoundInformations;
	Invalidate();
}
#pragma endregion

BOOL CG6TrucoView::PreCreateWindow(CREATESTRUCT& cs)
{
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
	if (controller.GetGamePoints()[0] >= 12 || controller.GetGamePoints()[1] >= 12) {
		start = false;
		DrawScoreBoard(pDC);
	}
	if (start) {
		DrawCards(pDC);
		DrawScoreBoard(pDC);
	}

	pDC->BitBlt(820, 400, cardH, cardW, &memDCDeck, 0, 0, SRCCOPY);

	UpdateButtons();
}

void CG6TrucoView::DrawOtherPlayerCards(CDC* pDC, Player* p, int x, int y) {
	CDC* card;
	for (int c = 0; c < p->GetHand().size(); c++) {
		if (c == p->GetSelectCardIndex()) {
			card = cardsMap[p->GetHand().at(c).GetSuit()][p->GetHand().at(c).GetRank()];
		}
		else {
			card = &memDCBack;
		}
		pDC->BitBlt(x, y, cardW, cardH, card, 0, 0, SRCCOPY);
		x += xOffset;
	}
}

void CG6TrucoView::DrawCurrentPlayerCards(CDC* pDC, Player* p)
{
	int x = 800;
	int y;
	CDC* card = &memDCBack;
	for (int c = 0; c < p->GetHand().size(); c++) {
		y = 650;
		if (p->playerType == Player::PlayerType::Human) {
			card = cardsMap[p->GetHand().at(c).GetSuit()][p->GetHand().at(c).GetRank()];
			if (cardClicked == (c + 1)) {
				y -= yOffset;
				if (hideCard) {
					card = &memDCBack;
				}
			}
		}
		pDC->BitBlt(x, y, cardW, cardH, card, 0, 0, SRCCOPY);
		cardArea.at(c) = CRect(x, y, x + cardW, y + cardH);
		x += xOffset;
	}
}

void CG6TrucoView::DrawScoreBoard(CDC* pDC)
{
	pDC->TextOut(1400, 70, L"Round Points");
	std::string score = "Team Human: ";
	score.append(std::to_string(currentRound->GetPoints().at(0)));
	pDC->TextOut(1400, 100, CString(score.c_str()));
	score = "Team Bot: ";
	score.append(std::to_string(currentRound->GetPoints().at(1)));
	pDC->TextOut(1400, 130, CString(score.c_str()));

	pDC->TextOut(1600, 70, L"Game Points");
	score = "Team Human: ";
	score.append(std::to_string(controller.GetGamePoints()[0]));
	pDC->TextOut(1600, 100, CString(score.c_str()));
	score = "Team Bot: ";
	score.append(std::to_string(controller.GetGamePoints()[1]));
	pDC->TextOut(1600, 130, CString(score.c_str()));
}

void CG6TrucoView::DrawCards(CDC* pDC) {
	std::vector<Player*> players;
	for (int a = 0; a < 4; a++) {
		players.push_back(currentRound->GetAllPlayers().at(a).get());
	}
	int n = currentRound->GetActivePlayerIndex();
	std::rotate(players.begin(), players.begin() + n, players.end());

	pDC->BitBlt(860, 320, cardW, cardH, cardsMap[currentRound->GetViraCard().GetSuit()][currentRound->GetViraCard().GetRank()], 0, 0, SRCCOPY);

	std::string bet = "Current Bet: ";
	bet.append(std::to_string(currentRound->GetCurrentBet()));
	SetStatusBarText(CString(bet.c_str()));

	DrawOtherPlayerCards(pDC, players.at(1), 40, 350);
	pDC->TextOut(140, 600, players.at(1)->GetPlayerName());

	DrawOtherPlayerCards(pDC, players.at(2), 800, 10);
	pDC->TextOut(880, 260, players.at(2)->GetPlayerName());

	DrawOtherPlayerCards(pDC, players.at(3), 1600, 350);
	pDC->TextOut(1700, 600, players.at(3)->GetPlayerName());

	pDC->TextOut(880, 900, players.at(0)->GetPlayerName());
	DrawCurrentPlayerCards(pDC, players.at(0));
}

void CG6TrucoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!currentRound->IsHumanPlayer()) {
		return;
	}
	Player* p = currentRound->GetActivePlayer();
	if (cardArea.at(2).PtInRect(point) && p->GetHand().size() >= 3)
	{
		if (cardClicked == 3) {
			cardClicked = 0;
		}
		else {
			cardClicked = 3;
		}
		hideCard = false;
		Invalidate();
	}
	else if (cardArea.at(1).PtInRect(point) && p->GetHand().size() >= 2) {
		if (cardClicked == 2) {
			cardClicked = 0;
		}
		else {
			cardClicked = 2;
		}
		hideCard = false;
		Invalidate();
	}
	else if (cardArea.at(0).PtInRect(point) && p->GetHand().size() >= 1) {
		if (cardClicked == 1) {
			cardClicked = 0;
		}
		else {
			cardClicked = 1;
		}
		hideCard = false;
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
	if (cardArea.at(2).PtInRect(point) && p->GetHand().size() >= 3)
	{
		cardClicked = 3;
		hideCard = true;
		Invalidate();
	}
	else if (cardArea.at(1).PtInRect(point) && p->GetHand().size() >= 2) {
		cardClicked = 2;
		hideCard = true;
		Invalidate();
	}
	else if (cardArea.at(0).PtInRect(point) && p->GetHand().size() >= 1) {
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

	CreateButton(buttonPlayCard, _T("Play Card"), CRect(1150, 750, 1300, 810), BUTTONPLAYCARDID);
	CreateButton(buttonAcceptTruco, _T("Accept"), CRect(600, 660, 750, 720), BUTTONACCEPTTRUCOID);
	CreateButton(buttonTruco, _T("Truco"), CRect(600, 750, 750, 810), BUTTONTRUCOID);
	CreateButton(buttonRejectTruco, _T("Reject"), CRect(600, 840, 750, 900), BUTTONREJECTTRUCOID);
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
			if (currentRound->GetCurrentTrucoCall() == Round::TrucoCallType::NONE) {
				//No Truco call - Normal Hand
				ShowTrucoButton();
				buttonPlayCard.ShowWindow(SW_SHOW);
				buttonAcceptTruco.ShowWindow(SW_HIDE);
				buttonRejectTruco.ShowWindow(SW_HIDE);
			}
			else if (currentRound->GetCurrentTrucoCall() == Round::TrucoCallType::CPU_ACCEPTED) {
				//CPU Accepts the Truco Call - Truco Hand
				buttonTruco.ShowWindow(SW_HIDE);
				buttonPlayCard.ShowWindow(SW_SHOW);
				buttonAcceptTruco.ShowWindow(SW_HIDE);
				buttonRejectTruco.ShowWindow(SW_HIDE);
			}
			else {
				//CPU Raised the bet - Decision Hand
				buttonTruco.ShowWindow(SW_HIDE);
				buttonPlayCard.ShowWindow(SW_HIDE);
				buttonAcceptTruco.ShowWindow(SW_SHOW);
				buttonRejectTruco.ShowWindow(SW_SHOW);
			}
		}
		else {
			//CPU turn
			buttonTruco.ShowWindow(SW_HIDE);
			buttonAcceptTruco.ShowWindow(SW_HIDE);
			buttonRejectTruco.ShowWindow(SW_HIDE);
			buttonPlayCard.ShowWindow(SW_HIDE);
		}
	}
	else {
		//New Game Main screen
		buttonNewGame.ShowWindow(SW_SHOW);
		buttonTruco.ShowWindow(SW_HIDE);
		buttonAcceptTruco.ShowWindow(SW_HIDE);
		buttonRejectTruco.ShowWindow(SW_HIDE);
		buttonPlayCard.ShowWindow(SW_HIDE);
	}
}

void CG6TrucoView::ShowTrucoButton() {
	//Hide Truco button when its on an "Eleven Hand" or the the current bet reach the maximum points
	if (currentRound->GetCurrentBet() < 12 && currentRound->GetPoints().at(0) < 11)
	{
		if (currentRound->GetCurrentBet() == 1) {
			buttonTruco.SetWindowTextW(L"Truco");
		}
		else {
			//Change button label when Truco was already called
			buttonTruco.SetWindowTextW(L"Raise Bet");
		}
		buttonTruco.ShowWindow(SW_SHOW);
	}
	else
	{
		buttonTruco.ShowWindow(SW_HIDE);
	}
}

void CG6TrucoView::OnButtonTrucoClicked()
{
	controller.RaiseBet();
}

void CG6TrucoView::OnButtonNewGameClicked()
{
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

void CG6TrucoView::OnButtonAcceptTrucoClicked()
{
	controller.AcceptBet();
}

void CG6TrucoView::OnButtonRejectTrucoClicked()
{
	controller.DenyBet();
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
