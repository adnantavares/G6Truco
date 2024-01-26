
// G6TrucoView.h : interface of the CG6TrucoView class
//

#pragma once
#include "Game.h"


class CG6TrucoView : public CView
{
private:
	CBrush backgroundBrush;
	CRect m_ImageRect;
	BOOL cardClicked = false;
	BOOL hideCard = false;

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

	//CBitmap* pOldBack;
	//CBitmap* pOldCard1;
	//CBitmap* pOldCard2;
	//CBitmap* pOldCard3;
	//CBitmap* pOldBmpDeck;
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

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
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
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in G6TrucoView.cpp
inline CG6TrucoDoc* CG6TrucoView::GetDocument() const
   { return reinterpret_cast<CG6TrucoDoc*>(m_pDocument); }
#endif

