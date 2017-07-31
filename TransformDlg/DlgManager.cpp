#include "stdafx.h"
#include "DlgManager.h"
#include "UnitDlg.h"
#include "ImgDLg.h"


CDlgManager::CDlgManager(bool fromLeft, long posY, long speed) :
	m_fromLeft(fromLeft),m_posY(posY), m_speed(speed < 0 ? -speed : speed)
{
}


CDlgManager::~CDlgManager()
{
}

void CDlgManager::DrawDlgs(CDC * pCDC)
{
	for (auto&  p : m_pMove) {
		p->DrawDlg(pCDC);
	}
}

void CDlgManager::UpdateMove()
{
	// add the new coming from ready to move
	bool bChanged = false;
	int i = 0;
	for (; i < m_pReady.size(); i++) 
	{
		auto& d = m_pReady[i];
		if ((m_fromLeft && d->GetDlgRc().right < m_rcWnd.left)
			|| (!m_fromLeft && d->GetDlgRc().left > m_rcWnd.right))
		{
			break;
		}
		
		bChanged = true;
		m_pMove.push_back(d);
	}
	if (bChanged)
		m_pReady.erase(m_pReady.begin(), m_pReady.begin() + i);
	// delete the dlg which is out of the region
	bChanged = false;
	int j = 0;
	for (; j < m_pMove.size(); j++)
	{
		if ((m_fromLeft && m_pMove[j]->GetDlgRc().left < m_rcWnd.right)
		||(!m_fromLeft && m_pMove[j]->GetDlgRc().right > m_rcWnd.left))
			break;

		bChanged = true;
		delete m_pMove[j];
		m_pMove[j] = nullptr;
	}
	if (bChanged)
		m_pMove.erase(m_pMove.begin(), m_pMove.begin() + j);
}

void CDlgManager::AddDlg()
{
	m_cnt++;
	CString path, title, text;
	path.Format(_T("%d.jpg"), (m_posY/100 * m_cnt) % 5);
	title.Format(_T("DLG %d"), m_cnt);

	int r = (m_posY/20 - m_cnt) % 3;
	if (r == 0)
		text = _T("你好！");
	else if (r == 1)
		text = _T("这是一段很长很长的文字， 这是对这个对话框的测试，看看效果怎么样");
	else
		text = _T("希望你玩的开心！！！");

	
	CUnitDlg* dlg = new CUnitDlg(path, title, text);

	SetDlgPosition(dlg);

	m_pReady.push_back(dlg);
}

void CDlgManager::AddImgDlg()
{
	m_cnt++;
	CString path;
	path.Format(_T("%d.jpg"), ((m_posY / 100-1) * m_cnt) % 5);
	CImgDLg * dlg = new CImgDLg(path);

	SetDlgPosition(dlg);
	m_pReady.push_back(dlg);
}




void CDlgManager::SetDlgPosition(CBaseDlg * dlg)
{
	long width = dlg->GetWidth();
	long startX = 0;

	if (m_pReady.empty())
	{
		if (m_pMove.empty())
		{
			startX = m_fromLeft ? -width : m_rcWnd.right;
		}
		else
		{
			long left = (m_pMove[m_pMove.size() - 1]->GetDlgRc()).left - 30;
			long right = (m_pMove[m_pMove.size() - 1]->GetDlgRc()).right + 30;
			startX = m_fromLeft ?
				(left > 0 ? -width : left - width) :
				(right < m_rcWnd.right ? m_rcWnd.right : right);
		}
	}
	else
	{
		long left = (m_pReady[m_pReady.size() - 1]->GetDlgRc()).left - 30;
		long right = (m_pReady[m_pReady.size() - 1]->GetDlgRc()).right + 30;
		startX = m_fromLeft ? left - width : right;
	}
	dlg->SetPosition(startX, m_posY);
}




void CDlgManager::Move()
{
	long speed = m_fromLeft ? m_speed : -m_speed;
	for (auto & d : m_pReady) {
		d->Move(speed);
	}
	for (auto & d : m_pMove) {
		d->Move(speed);
	}

	UpdateMove();
}
