
// TransformDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "TransformDlg.h"
#include "TransformDlgDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int WNDH = 80; //ˮƽ��Ļ��С+���¸߶�
const int FONTSZ = 20; // ��ʾ�����ִ�С
const int PADDING = 20;


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
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTransformDlgDlg ��Ϣ�������

BOOL CTransformDlgDlg::OnInitDialog()
{
	GdiplusStartupInput Startup;
	GdiplusStartup(&m_Token, &Startup, NULL);

	m_hMemDC = 0;
	//m_bgImg = Image::FromFile(L"bg.jpg");
	m_bgImg.Load(L"bg.jpg"); // load the background image

	


	CDialogEx::OnInitDialog();
//	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
//	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
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
	m_btn1 = { rcSave.right - 270,rcSave.bottom - 100,  rcSave.right - 220, rcSave.bottom - 50 };
	m_btn2 = { rcSave.right - 200,rcSave.bottom - 100,  rcSave.right - 150, rcSave.bottom - 50 };
	m_btn3 = { rcSave.right - 130,rcSave.bottom - 100,  rcSave.right - 80, rcSave.bottom - 50 };

	m_mng1 = new CDlgManager(false, 200, 3);
	m_mng1->SetWindow(rcSave);

	m_mng2 = new CDlgManager(false, 300, 4);
	m_mng2->SetWindow(rcSave);

	m_mng3 = new CDlgManager(false, 500, 3);
	m_mng3->SetWindow(rcSave);

	// start moving at the very beginning
	OnMovestart();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}





// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTransformDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		//......
	}
	else
	{
		////CDialog::OnPaint(); //��Ҫ�������
		CPaintDC dc(this);//�Ի����dc//ͨ��CPaintDC������ӦWM_PAINT��Ϣ��
		//				  //CPaintDC�Ǵ�CDC���������ģ��ڹ���ʱ�Զ�����CWnd::BeginPaint,����ʱ����CWnd::EndPaint��

		//CPaintDC paintDC(this);     // device context for painting
		//CDoubleBufferDC dc(&paintDC); // device context for painting

		RECT rect;// �ͻ�������
		GetClientRect(&rect);		

		// ʹ��˫���������Ļˢ��ʱ��˸
		CDC dcMem;// �ڴ�dc
		CBitmap bmpMem; // λͼ
		dcMem.CreateCompatibleDC(NULL);// ��������dc
		bmpMem.CreateCompatibleBitmap(&dc, rect.right - rect.left, rect.bottom - rect.top);//�������ͻ������Сһ����(��)λͼ
																						   // ��λͼѡ���豸�����Ļ�����
		CBitmap *pOld = dcMem.SelectObject(&bmpMem);


		//dcMem.FillSolidRect(&rect, RGB(255, 255, 255));
		//FillDCRect(dcMem, &rect, 0xffffff);
		//background
		dcMem.SetStretchBltMode(COLORONCOLOR);//�����ã�ʹͼƬ���ź���ɫ��ʧ��
		m_bgImg.Draw(dcMem, rect);
		
		{
			dcMem.FillSolidRect(&m_btn1, RGB(255, 255, 0));
			dcMem.FillSolidRect(&m_btn2, RGB(0, 255, 255));
			dcMem.FillSolidRect(&m_btn3, RGB(255, 0, 255));
		}

		//// draw dialog
		m_mng1->DrawDlgs(&dcMem);
		m_mng2->DrawDlgs(&dcMem);
		m_mng3->DrawDlgs(&dcMem);


		//// ���ڴ濽�����豸dc
		dc.BitBlt(0, 0, rect.right - rect.left, rect.bottom - rect.top, &dcMem, 0, 0, SRCCOPY);
		////dc.StretchBlt(0, 0, rect.right - rect.left, rect.bottom - rect.top, &dcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SRCCOPY);

		dc.SelectObject(pOld);
		// �ͷ���Դ
		bmpMem.DeleteObject();
		dcMem.DeleteDC();
	}

	if (TRUE == m_bgChange)
	{
		m_bgChange = FALSE;
		ModifyStyle(0, WS_CLIPCHILDREN);
	}

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
	//SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTransformDlgDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (point.x < m_btn1.right && point.x > m_btn1.left && point.y < m_btn1.bottom && point.y > m_btn1.top)
	{
		m_mng1->AddDlg();
	}
	if (point.x < m_btn2.right && point.x > m_btn2.left && point.y < m_btn2.bottom && point.y > m_btn2.top)
	{
		m_mng2->AddDlg();
	}
	if (point.x < m_btn3.right && point.x > m_btn3.left && point.y < m_btn3.bottom && point.y > m_btn3.top)
	{
		m_mng3->AddImgDlg();
	}
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
		// move dialog
		m_mng1->Move();
		m_mng2->Move();
		m_mng3->Move();
		//RECT rc = {0, 200, m_rcClient.Width(), 380};
		//InvalidateRect(&rc);
		Invalidate();
	}
	else if (nIDEvent == 2)
	{
		m_mng2->Move();
		RECT rc = { 0, 300, m_rcClient.Width(), 380 };
		InvalidateRect(&rc);
		//Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTransformDlgDlg::OnMovestart()
{
	SetTimer(1, 10, NULL);
	//SetTimer(2, 5, NULL);
}


void CTransformDlgDlg::OnMovestop()
{
	KillTimer(1);
	//KillTimer(2);
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
