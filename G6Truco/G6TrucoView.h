
// G6TrucoView.h : interface of the CG6TrucoView class
//

#pragma once
#include "Game.h"
#include "string"
#include "TrucoController.h"

#define BUTTONTRUCOID 10000
#define BUTTONNEWGAMEID 10001
#define BUTTONACCEPTTRUCOID 10002
#define BUTTONREJECTTRUCOID 10003
#define BUTTONPLAYCARDID 10004

class CG6TrucoView : public CView
{
private:
	TrucoController controller;

	CBrush backgroundBrush;
	CRect m_Card1Rect;
	CRect m_Card2Rect;
	CRect m_Card3Rect;
	int cardClicked = 0;
	BOOL hideCard = false;
	int cardH = 240;
	int cardW = 165;
	bool start = false;

	CDC memDCBack;
	CDC memDCCard1;
	CDC memDCCard2;
	CDC memDCCard3;
	CDC memDCDeck;

	CImage imageCardBack;
	CImage imageCard1;
	CImage imageCard2;
	CImage imageCard3;
	CImage imageDeck;

	CBitmap bmpBack;
	CBitmap bmpCard1;
	CBitmap bmpCard2;
	CBitmap bmpCard3;
	CBitmap bmpDeck;

	CButton buttonTruco;
	CButton buttonNewGame;
	CButton buttonAcceptTruco;
	CButton buttonRejectTruco;
	CButton buttonPlayCard;

	void CreateButton(CButton& button, LPCTSTR contentText, CRect rectButton, int idButton);
	//CBitmap* pOldBack;
	//CBitmap* pOldCard1;
	//CBitmap* pOldCard2;
	//CBitmap* pOldCard3;
	//CBitmap* pOldBmpDeck;
	void DrawCards(CDC* pDC);
	void DrawPlayerCards(CDC* pDC, Player* p, int x, int y);
	void DrawScoreBoard(CDC* pDC);
	void UpdateButtons();
	void SetStatusBarText(const CString& strText);
	const std::string cardsNameMap[4][10] = { 
		{"c2","c3","c4","c5","c6","c7","cj","cq","ck","ca"},
		{"e2","e3","e4","e5","e6","e7","ej","eq","ek","ea"},
		{"o2","o3","o4","o5","o6","o7","oj","oq","ok","oa"},
		{"p2","p3","p4","p5","p6","p7","pj","pq","pk","pa"} };
	CDC* cardsMap[4][10];
	Round* currentRound;

#pragma region Events
	void OnActivePlayerChangedEvent(Player* player);
	void OnRoundInformationsChangedEvent(Round* currentRoundInformations);
#pragma endregion


protected: // create from serialization only
	CG6TrucoView() noexcept;
	DECLARE_DYNCREATE(CG6TrucoView)

	// Attributes
public:
	CG6TrucoDoc* GetDocument() const;

	// Operations
public:
	void drawGame(const Game& game); // Draws the current state of the game.
	void updateView(); // Updates the view with changes in the game state.
	void showBetOptions(); // Shows betting options to the player.
	void OnBnClickedRaiseBet();
	void CreateButton();
	void TryPlayCard(int cardIndex);

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
protected:

	// Implementation
public:
	virtual ~CG6TrucoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	void OnButtonTrucoClicked();
	void OnButtonNewGameClicked();
	void OnButtonPlayCardClicked();
	void OnButtonAcceptTrucoClicked();
	void OnButtonRejectTrucoClicked();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in G6TrucoView.cpp
inline CG6TrucoDoc* CG6TrucoView::GetDocument() const
{
	return reinterpret_cast<CG6TrucoDoc*>(m_pDocument);
}
#endif

