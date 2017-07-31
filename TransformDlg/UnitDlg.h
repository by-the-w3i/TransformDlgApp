#pragma once
#include "BaseDlg.h"

using namespace Gdiplus;

class CUnitDlg : public CBaseDlg
{
public:
	//CUnitDlg()=delete;
	~CUnitDlg();

	CUnitDlg(CString path, CString title, CString message);

	void DrawDlg(CDC  * pCDC) override;

private:

	CString m_title;
	CString m_msg;

	int m_padding = 0;

	int m_fontW = 10;
	int m_fontH = 20;
	HFONT m_hFont;
};



