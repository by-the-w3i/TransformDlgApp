#pragma once
#include "BaseDlg.h"
#include <vector>

class CDlgManager
{
public:
	CDlgManager()=delete;
	CDlgManager(bool fromLeft, long posY, long speed);
	~CDlgManager();

	void DrawDlgs(CDC * pCDC);

	// update the move list add/delete dlgs
	void UpdateMove();

	// add a dlg to ready list
	void AddDlg();
	void AddImgDlg();
	void SetDlgPosition(CBaseDlg *);

	void Move();

	void SetWindow(CRect wnd) { m_rcWnd = wnd; };


private:
	std::vector<CBaseDlg * > m_pMove;
	std::vector<CBaseDlg * > m_pReady;

	CRect m_rcWnd;

	long m_cnt = 0;

	bool m_fromLeft = true;
	long m_speed = 3;
	long m_posY = 0;
};

