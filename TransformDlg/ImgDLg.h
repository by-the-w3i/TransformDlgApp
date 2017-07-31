#pragma once
#include "BaseDlg.h"

class CImgDLg : public CBaseDlg
{
public:
	CImgDLg() = delete;
	CImgDLg(CString path);
	~CImgDLg();

	void DrawDlg(CDC * pCDC) override;
};

