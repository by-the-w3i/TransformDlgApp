
// TransformDlgDlg.h : ͷ�ļ�
//

#pragma once


// CTransformDlgDlg �Ի���
class CTransformDlgDlg : public CDialogEx
{
// ����
public:
	CTransformDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORMDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	CRect m_rcClient;
	HFONT m_hFont;
	CString m_strShowText;

	RECT m_rcDraw;
	RECT m_rcOver;
	//int m_nFontHeight;	
	BOOL m_bMouseOver;
	BOOL m_bSizeChange;

	HDC m_hMemDC;
	//HBITMAP m_hMemBitmap;
	POINT m_ptStart;

	CImage m_imgA;
	//CImage m_imgB;


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnIdmQuitx();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMovestart();
	afx_msg void OnMovestop();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
