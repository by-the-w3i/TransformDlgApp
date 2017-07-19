
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

const double MOVE_UNIT = 1.0;
const int WNDH = 100; //水平屏幕大小+上下高度
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
END_MESSAGE_MAP()


// CTransformDlgDlg 消息处理程序

BOOL CTransformDlgDlg::OnInitDialog()
{
	//		CRect m_rcClient;
	//		HFONT m_hFont;
	m_strShowText = _T("通过CBitmap,HBITMAP,直接用OnPaint()绘制！");
	m_strShowTitle = _T("请填入用户名！");
	//		RECT m_rcDraw;
	//		RECT m_rcOver;
	//		int m_nFontHeight;
	m_hMemDC = 0;
	//m_hMemBitmap = 0;
	m_ptStart.x = 0;
	m_ptStart.y = 10;

	m_imgA.Load(TEXT("L.jpg"));

	CDialogEx::OnInitDialog();
//	SetIcon(m_hIcon, TRUE);			// 设置大图标
//	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | 0x80000);
	SetLayeredWindowAttributes(0xffffff, 100, LWA_COLORKEY);
	//m_nFontHeight = 50;
	m_hFont = ::CreateFont(FONTSZ, 0, 0, 0, 900,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	/// set dialog window
	{
		m_rcDlg.left = 0;
		m_rcDlg.right = m_strShowText.GetLength()*FONTSZ;
		m_rcDlg.top = 0;
		m_rcDlg.bottom = WNDH;
	}
	
	CRect rcSave;
	{
		int nxScreen = ::GetSystemMetrics(SM_CXSCREEN);
		int nyScreen = ::GetSystemMetrics(SM_CYSCREEN);
		rcSave.left = 0;
		rcSave.right = nxScreen;
		rcSave.top = (nyScreen- WNDH)/2;
		rcSave.bottom = rcSave.top+ WNDH;
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
	// background
	{
		FillRect(hMemDc, &m_rcDlg, CreateSolidBrush(RGB(50, 151, 151)));
		MoveToEx(hMemDc, m_rcClient.left + WNDH + PADDING, WNDH / 2.0, (LPPOINT)NULL);
		LineTo(hMemDc, m_rcDlg.right, WNDH / 2.0);
	}
	// profile img
	{
		m_imgA.Draw(hMemDc, m_rcDlg.left,m_rcDlg.top, WNDH, WNDH);
	}
	// title
	{
		::SelectObject(hMemDc, m_hFont);
		::SetTextColor(hMemDc, 0x000000);
		::SetBkMode(hMemDc, TRANSPARENT);
		CRect rcText(&m_rcClient);
		rcText.left += WNDH + +PADDING;
		rcText.top -= WNDH / 2;
		::DrawText(hMemDc, m_strShowTitle, m_strShowTitle.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	/// text
	{
		//::SelectObject(hMemDc, m_hFont);
		::SetTextColor(hMemDc, 0x0000ff);
		//::SetBkMode(hMemDc, TRANSPARENT);
		CRect rcText(&m_rcClient);
		rcText.left += WNDH + +PADDING;
		rcText.top += WNDH/2;
		::DrawText(hMemDc, m_strShowText, m_strShowText.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	// clear the buffer
	BitBlt(dc.m_hDC, 0, 0, rc.right, rc.bottom, hMemDc, 0, 0, SRCCOPY);
	SelectObject(hMemDc, hOldBmp);
	DeleteObject(hBmp);
	DeleteObject(hMemDc);

	//dc.Rectangle(0,0, m_strShowText.GetLength()*FONTSZ, WNDH);
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
		/// update the position
		m_rcClient.left += MOVE_UNIT;
		m_ptStart.x += MOVE_UNIT;
		m_rcDlg.left += MOVE_UNIT;
		m_rcDlg.right += MOVE_UNIT;
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