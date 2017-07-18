
// TransformDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <vector>

#include "TransformDlg.h"
#include "TransformDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransformDlgDlg �Ի���

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
			throw  _T("DC���Ϊ�գ�");
		if (NULL == lpRect)
			throw _T("��������ָ��Ϊ�գ�");
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
END_MESSAGE_MAP()


// CTransformDlgDlg ��Ϣ�������

BOOL CTransformDlgDlg::OnInitDialog()
{
	//		CRect m_rcClient;
	//		HFONT m_hFont;
	m_strShowText = _T("ͨ��CBitmap,HBITMAP,ֱ����OnPaint()���ƣ�");
	//		RECT m_rcDraw;
	//		RECT m_rcOver;
	//		int m_nFontHeight;
	m_hMemDC = 0;
	m_hMemBitmap = 0;
	m_ptStart.x = 0;
	m_ptStart.y = 10;

	m_imgA.Load(TEXT("1.jpg"));
	m_imgB.Load(TEXT("2.jpg"));

	CDialogEx::OnInitDialog();
//	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
//	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | 0x80000);
	SetLayeredWindowAttributes(0xffffff, 100, LWA_COLORKEY);
	m_hFont = ::CreateFont(50, 0, 0, 0, 900,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("����"));
	m_nFontHeight = 50;
	CRect rcSave;
	{
		int nxScreen = ::GetSystemMetrics(SM_CXSCREEN);
		int nyScreen = ::GetSystemMetrics(SM_CYSCREEN);
		rcSave.left = 0;
		rcSave.right = nxScreen;
		int nHight = 500; //ˮƽ��Ļ��С+���¸߶�500
		rcSave.top = (nyScreen- nHight)/2;
		rcSave.bottom = rcSave.top+ nHight;
	}
	MoveWindow(&rcSave);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTransformDlgDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// ��Ϊ��ͼ��Ϣ����
	if(NULL==m_hMemDC)
	{
		m_hMemDC=CreateCompatibleDC(dc.m_hDC);
		::SetBkMode(m_hMemDC,TRANSPARENT);
	}
	if (m_bSizeChange)
	{
		ReleaseGDIHandle((HANDLE*)&m_hMemBitmap);
		m_hMemBitmap = CreateCompatibleBitmap(dc.m_hDC, m_rcClient.Width(), m_rcClient.Height());
		::SelectObject(m_hMemDC, m_hMemBitmap);
		m_bSizeChange = FALSE;
	}

	HFONT hOldFont=(HFONT)::SelectObject(m_hMemDC,m_hFont);

	//FillDCRect(m_hMemDC,&m_rcClient,0xff00ff);
	FillDCRect(m_hMemDC,&m_rcClient,0xffffff);

	//draw icon
	{
		m_imgA.Draw(m_hMemDC, m_ptStart.x, m_ptStart.y, m_imgA.GetWidth(), m_imgA.GetHeight());
		m_imgB.Draw(m_hMemDC, m_ptStart.x+50, m_ptStart.y, m_imgB.GetWidth(), m_imgB.GetHeight());
	}
	{
		::SetTextColor(m_hMemDC,0x0000ff);
		::SetBkMode(m_hMemDC,TRANSPARENT);
		CRect rcText(m_rcClient);
		rcText.left += 120;
		::DrawText(m_hMemDC,m_strShowText,m_strShowText.GetLength(),&rcText,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	::BitBlt(dc.m_hDC,0,0,m_rcClient.Width(),m_rcClient.Height(),\
		m_hMemDC,0,0,SRCCOPY);
	::SelectObject(m_hMemDC,hOldFont);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

//		m_imgA.Draw(m_hMemDC, m_ptStart.x, m_ptStart.y, m_imgA.GetWidth(), m_imgA.GetHeight());
//		m_imgB.Draw(m_hMemDC, m_ptStart.x + 50, m_ptStart.y, m_imgB.GetWidth(), m_imgB.GetHeight());
		RECT rcDraw;
		rcDraw.left = m_ptStart.x;
		rcDraw.top = m_ptStart.y;
		rcDraw.right = m_ptStart.x + 50+m_imgB.GetWidth();
		rcDraw.bottom = m_ptStart.y + m_imgA.GetHeight();

		InvalidateRect(&rcDraw);

		m_ptStart.x += 1;

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
