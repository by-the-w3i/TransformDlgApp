#pragma once
class CBaseDlg
{
public:
	CBaseDlg() = delete;
	CBaseDlg(const CBaseDlg &) = delete;

	CBaseDlg(CString path);
	virtual ~CBaseDlg();

	void SetPosition(LONG startX, LONG startY);

	void Move(long speed); // 从左往右的方向为正方向

	virtual void DrawDlg(CDC  * pCDC) = 0;

	CRect GetDlgRc() { return m_rcDlg; }
	INT GetWidth() { return m_width; }

protected:
	CImage m_img;

	int m_width;
	int m_height;
	int m_imgPad;

	CRect m_rcDlg; // the rectangle dialog interface
};

