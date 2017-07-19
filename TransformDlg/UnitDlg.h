#pragma once
class CUnitDlg
{
public:
	CUnitDlg()=delete;
	~CUnitDlg();

	CUnitDlg(LPCTSTR path, CString title, CString message, DOUBLE speed);

	//void Draw()

	// Setter
	void SetPosition(LONG l, LONG r, LONG t, LONG b);
	// Getter


public:
	LONG left;
	LONG right;
	LONG top;
	LONG bottom;

private:
	CImage m_img;
	CString m_title;
	CString m_msg;

	DOUBLE m_width;
	DOUBLE m_speed;

};



