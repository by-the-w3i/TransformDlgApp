
// TransformDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <vector>

#include "TransformDlg.h"
#include "TransformDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double MOVE_UNIT = 2.0;
const int WNDH = 80; //水平屏幕大小+上下高度
const int FONTSZ = 20; // 显示的文字大小
const int PADDING = 20;


// CTransformDlgDlg 对话框

void ReleaseGDIHandle(HANDLE* pHOBJ)
{
	if (NULL != pHOBJ&&NULL != *pHOBJ)
	{
		::DeleteObject(*pHOBJ);
		*pHOBJ = NULL;
	}
}

bool FillDCRect(IN HDC hDC, IN LPRECT lpRect, IN COLORREF color)
{
	bool bRet = false;

	{
		if (NULL == hDC)
			throw  _T("DC句柄为空！");
		if (NULL == lpRect)
			throw _T("矩形区域指针为空！");
		::SetBkColor(hDC, color);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		bRet = true;
	}


	return bRet;
}

CTransformDlgDlg::CTransformDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRANSFORMDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransformDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransformDlgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
//	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_IDM_QUITX, &CTransformDlgDlg::OnIdmQuitx)
	ON_WM_TIMER()
	ON_COMMAND(ID_MOVESTART, &CTransformDlgDlg::OnMovestart)
	ON_COMMAND(ID_MOVESTOP, &CTransformDlgDlg::OnMovestop)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTransformDlgDlg 消息处理程序

BOOL CTransformDlgDlg::OnInitDialog()
{
	GdiplusStartupInput Startup;
	GdiplusStartup(&m_Token, &Startup, NULL);

	m_hMemDC = 0;

	/// initialize the testing dlg
	{
		pDlg1 =new CUnitDlg(TEXT("L.jpg"), _T("DLG 1"), _T("这是对 UnitDlg class 的测试"));
		pDlg1->SetPosition(100, 50);
		pDlg2 = new CUnitDlg(TEXT("2.jpg"), _T("DLG 2"), _T("This is the test of the dialog 2"));
		pDlg2->SetPosition(150, 200);
	}

	CDialogEx::OnInitDialog();
//	SetIcon(m_hIcon, TRUE);			// 设置大图标
//	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | 0x80000);
	SetLayeredWindowAttributes(0xffffff, 100, LWA_COLORKEY);

	CRect rcSave;
	{
		int nxScreen = ::GetSystemMetrics(SM_CXSCREEN);
		int nyScreen = ::GetSystemMetrics(SM_CYSCREEN);
		rcSave.left = 0;
		rcSave.right = nxScreen;
		rcSave.top = 0;
		rcSave.bottom = nyScreen;
	}
	MoveWindow(&rcSave);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTransformDlgDlg::OnPaint()
{
	
	CPaintDC dc(this);
	RECT rc;
	GetClientRect(&rc);

	HDC hMemDc = CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmp = CreateCompatibleBitmap(dc.m_hDC, rc.right, rc.bottom);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDc, hBmp);


	//FillDCRect(hMemDc,&rc,0xff00ff); 
	FillDCRect(hMemDc, &rc, 0xffffff);

	/// DRAW here 
	{
		pDlg1->Draw(hMemDc);
		pDlg2->Draw(hMemDc);
	}

	// clear the buffer
	BitBlt(dc.m_hDC, 0, 0, rc.right, rc.bottom, hMemDc, 0, 0, SRCCOPY);
	SelectObject(hMemDc, hOldBmp);
	DeleteObject(hBmp);
	DeleteObject(hMemDc);

}



//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTransformDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTransformDlgDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
	{
		GetClientRect(&m_rcClient);
		m_rcDraw = m_rcClient;
		m_rcOver = m_rcClient;
	}
	Invalidate();
	m_bSizeChange = TRUE;


	// TODO: Add your message handler code here
}


//void CTransformDlgDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialogEx::OnNcMouseMove(nHitTest, point);
//}


void CTransformDlgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

/*	if (m_rcClient.PtInRect(point))
		m_bMouseOver = TRUE;
	else
		m_bMouseOver = FALSE;
	Invalidate();
*/
	CDialogEx::OnMouseMove(nFlags, point);
}


void CTransformDlgDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CDialogEx::OnRButtonDblClk(nFlags, point);
}


void CTransformDlgDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt;
	::GetCursorPos(&pt);
	CMenu menu, *pMenu = NULL;
	menu.LoadMenu(IDR_MENU1);
	pMenu = menu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	pMenu->DestroyMenu();

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CTransformDlgDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CTransformDlgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTransformDlgDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTransformDlgDlg::OnIdmQuitx()
{
	DestroyWindow();
	// TODO: Add your command handler code here
}


void CTransformDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		pDlg1->Move(MOVE_UNIT);
		pDlg2->Move(MOVE_UNIT);
		Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTransformDlgDlg::OnMovestart()
{
	SetTimer(1, 20,NULL);
}


void CTransformDlgDlg::OnMovestop()
{
	KillTimer(1);
	// TODO: Add your command handler code here
}

BOOL CTransformDlgDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTransformDlgDlg::OnDestroy()
{
	GdiplusShutdown(m_Token);
}
