#pragma once
#include "UnitDlg.h"
#include <vector>

class CDlgManager
{
public:
	CDlgManager()=delete;
	CDlgManager(bool fromLeft, long posY, long speed, bool withImg = false);
	~CDlgManager();

	void DrawDlgs(CDC * pCDC);

	// update the move list add/delete dlgs
	void UpdateMove();

	// add a dlg to ready list
	void AddDlg();
	void SetDlgPosition(CUnitDlg *);

	void Move();

	void SetWindow(CRect wnd) { m_rcWnd = wnd; };


private:
	std::vector<CUnitDlg * > m_pMove;
	std::vector<CUnitDlg * > m_pReady;

	CRect m_rcWnd;

	long m_cnt = 0;

	bool m_fromLeft = true;
	long m_speed = 3;
	long m_posY = 0;

	bool m_withImg = false;
};

