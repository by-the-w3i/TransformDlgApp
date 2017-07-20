#pragma once

using namespace Gdiplus;

class CUnitDlg
{
public:
	CUnitDlg()=delete;
	~CUnitDlg();

	CUnitDlg(LPCTSTR path, CString title, CString message);

	void SetPosition(LONG startX, LONG startY);

	void Draw(HDC & hMemDc);
	void Move(INT speed); // 从左往右的方向为正方向

private:
	// CImage m_img;
	Image* m_pImage;
	//CImage nImage;
	//CBitmap m_bmp;
	CString m_title;
	CString m_msg;

	DOUBLE m_width;

	HFONT m_hFont;

	CRect m_rcDlg; // the rectangle dialog interface
};



