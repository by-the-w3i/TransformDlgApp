#include "stdafx.h"
#include "UnitDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double MOVE_UNIT = 1.0;
const int WNDH = 80; //水平 dialog 高度
const int FONTSZ = 20; // 文字高度
const int FONTW = 10; // 文字宽度
const int PADDING = 0;

CUnitDlg::CUnitDlg(LPCTSTR path, CString title, CString message) :
	m_title(title), m_msg(message)
{
	CString strFile = path;
	m_pImage = Image::FromFile(strFile);

	m_hFont = ::CreateFont(FONTSZ, FONTW, 0, 0, 900,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	m_width = (title.GetLength() > message.GetLength() ? title.GetLength() : message.GetLength()) * FONTW 
		+ PADDING *2 + WNDH*3;

	// default start at (0, 0)
	SetPosition(0, 0);
}



void CUnitDlg::SetPosition(LONG startX, LONG startY)
{
	m_rcDlg.left = startX;
	m_rcDlg.right = startX+m_width;
	m_rcDlg.top = startY;
	m_rcDlg.bottom = startY+WNDH;
}


void CUnitDlg::Draw(HDC & hMemDc)
{
	// background
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		HBRUSH hBrush = CreateSolidBrush(RGB(128, 255, 128));
		HGDIOBJ hOldPen = ::SelectObject(hMemDc, hPen);
		HGDIOBJ hOldBrush = ::SelectObject(hMemDc, hBrush);
		::RoundRect(hMemDc, m_rcDlg.left, m_rcDlg.top, m_rcDlg.right, m_rcDlg.bottom, WNDH, WNDH);
		MoveToEx(hMemDc, m_rcDlg.left + WNDH + PADDING, (m_rcDlg.top+m_rcDlg.bottom) / 2.0, (LPPOINT)NULL);
		LineTo(hMemDc, m_rcDlg.right-PADDING, (m_rcDlg.top + m_rcDlg.bottom) / 2.0);
		::DeleteObject(::SelectObject(hMemDc, hOldPen));
		::DeleteObject(::SelectObject(hMemDc, hOldBrush));
	}
	// profile img
	{
		HRGN rgn = CreateEllipticRgn(m_rcDlg.left + 1, m_rcDlg.top + 1, m_rcDlg.left + 1 + WNDH, m_rcDlg.top + 1 + WNDH);
		Region region(rgn);
		Graphics graph(hMemDc);
		SolidBrush greenBrush(Color(255, 255, 255, 255));
		Pen blackPen(Color(255, 0, 0, 0), 1);
		graph.DrawEllipse(&blackPen, m_rcDlg.left, m_rcDlg.top, WNDH, WNDH);
		graph.SetClip(&region, CombineModeReplace);
		graph.DrawImage(m_pImage, m_rcDlg.left, m_rcDlg.top, WNDH, WNDH);	
	}

	// title
	{
		::SelectObject(hMemDc, m_hFont);
		::SetTextColor(hMemDc, 0x000000);
		::SetBkMode(hMemDc, TRANSPARENT);
		CRect rcText(&m_rcDlg);
		rcText.left += WNDH +PADDING;
		rcText.top -= WNDH / 2;
		::DrawText(hMemDc, m_title, m_title.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	/// text
	{
		::SetTextColor(hMemDc, 0x0000ff);
		CRect rcText(&m_rcDlg);
		rcText.left += WNDH +PADDING;
		rcText.top += WNDH / 2;
		::DrawText(hMemDc, m_msg, m_msg.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

void CUnitDlg::Move(INT speed)
{
	m_rcDlg.left += speed;
	m_rcDlg.right += speed;
}


CUnitDlg::~CUnitDlg()
{
}
