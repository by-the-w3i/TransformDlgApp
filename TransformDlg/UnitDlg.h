#pragma once

const long MIN_IMGW = 400;
const long MAX_IMGW = 450;
const long FIX_IMGH = 300;
const int FONTW = 10;
const int FONTH = 20;
const int TEXT2IMG = 20;


// one UnitDlg should send 1 text and 1/0 picture
class CUnitDlg
{
public:
	CUnitDlg() = delete;
	CUnitDlg(const CUnitDlg &) = delete;

	CUnitDlg(CString icon, CString title, CString msg, CString img = L"");

	~CUnitDlg();

	void SetPosition(long startX, long startY);

	void Draw(CDC * pCDC);

	void BestFitImg(CDC & cdcMEM);
	
	void Move(long speed);

	CRect GetDlgRc() { return m_globalArea; }
	long GetWidth() { return m_width; }
	long GetHeight() { return m_height; }


private:
	// the area width which is determined by either the text width or picture fixed width
	long m_width;
	long m_height;
	CRect m_globalArea; // relative to the main DC



	HFONT m_hFont;

	long m_textW;
	long m_textH;

	CImage m_icon;
	CString m_title;
	CString m_msg;


	// IMAGE PART config
	CImage m_img;
	long m_imgW = 0;
	long m_imgH;
	int m_imgPad;
	// image margin left
	int m_imgML;
};

