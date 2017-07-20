
// TransformDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "UnitDlg.h"

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

	RECT m_rcDraw;
	RECT m_rcOver;
	//int m_nFontHeight;	
	BOOL m_bMouseOver;
	BOOL m_bSizeChange;

	HDC m_hMemDC;
	
	/// test dlg
	CUnitDlg * pDlg1;
	CUnitDlg * pDlg2;

	ULONG_PTR m_Token;


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
	afx_msg void OnDestroy();
};
