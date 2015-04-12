// SceneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "math.h"
#include "MyPhysics.h"
#include "SceneDlg.h"


// CSceneDlg 对话框

IMPLEMENT_DYNAMIC(CSceneDlg, CDialog)

CSceneDlg::CSceneDlg(float G,float Ldamping, float Adamping, float Mu, float Bounce, CWnd* pParent /*=NULL*/)
: CDialog(CSceneDlg::IDD, pParent)
{
	g = G;
	ng = G*10;
	ldamping = Ldamping;
	nldamping = Ldamping * 100;
	adamping = Adamping;
	nadamping = Adamping * 100;
	mu = Mu;
	if(Mu == (float)HUGE_VAL)
	{
		mu_infinity = TRUE;
		nmu = 100;
	}
	else
	{
		mu_infinity = FALSE;
		nmu = Mu * 100;
	}
	r = Bounce;
	nr = Bounce * 100;
}

CSceneDlg::~CSceneDlg()
{
}

void CSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_G, g);
	DDX_Slider(pDX, IDC_SLIDER_G, ng);
	DDX_Text(pDX, IDC_EDIT_LDAMPING, ldamping);
	DDX_Text(pDX, IDC_EDIT_ADAMPING, adamping);
	DDX_Slider(pDX, IDC_SLIDER_ADAMPING, nadamping);
	DDX_Slider(pDX, IDC_SLIDER_LDAMPING, nldamping);
	DDX_Text(pDX, IDC_EDIT_MU, mu);
	DDX_Slider(pDX, IDC_SLIDER_MU, nmu);
	DDX_Text(pDX, IDC_EDIT_R, r);
	DDX_Slider(pDX, IDC_SLIDER_R, nr);
	DDX_Check(pDX, IDC_CHECK_INFINITY, mu_infinity);
}


BEGIN_MESSAGE_MAP(CSceneDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_G, &CSceneDlg::OnEnKillfocusEditG)
	ON_EN_KILLFOCUS(IDC_EDIT_LDAMPING, &CSceneDlg::OnEnKillfocusEditLdamping)
	ON_EN_KILLFOCUS(IDC_EDIT_ADAMPING, &CSceneDlg::OnEnKillfocusEditAdamping)
	ON_EN_KILLFOCUS(IDC_EDIT_MU, &CSceneDlg::OnEnKillfocusEditMu)
	ON_EN_KILLFOCUS(IDC_EDIT_R, &CSceneDlg::OnEnKillfocusEditR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_G, &CSceneDlg::OnNMCustomdrawSliderG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LDAMPING, &CSceneDlg::OnNMCustomdrawSliderLdamping)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ADAMPING, &CSceneDlg::OnNMCustomdrawSliderAdamping)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MU, &CSceneDlg::OnNMCustomdrawSliderMu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_R, &CSceneDlg::OnNMCustomdrawSliderR)
	ON_BN_CLICKED(IDC_CHECK_INFINITY, &CSceneDlg::OnBnClickedCheckInfinity)
END_MESSAGE_MAP()


// CSceneDlg 消息处理程序

void CSceneDlg::OnEnKillfocusEditG()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ng = 10 * g;
	UpdateData(FALSE); // output data
}

void CSceneDlg::OnEnKillfocusEditLdamping()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nldamping = 100 * ldamping;
	UpdateData(FALSE); // output data
}

void CSceneDlg::OnEnKillfocusEditAdamping()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nadamping = 100 * adamping;
	UpdateData(FALSE); // output data
}

void CSceneDlg::OnEnKillfocusEditMu()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	if(mu_infinity == TRUE)
	{
		mu = HUGE_VAL;
	}
	else
	{
		nmu = 100 * mu;
	}
	UpdateData(FALSE); // output data
}

void CSceneDlg::OnEnKillfocusEditR()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nr = 100 * r;
	UpdateData(FALSE); // output data
}

void CSceneDlg::OnNMCustomdrawSliderG(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	g = ng / 10.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSceneDlg::OnNMCustomdrawSliderLdamping(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ldamping = nldamping / 100.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSceneDlg::OnNMCustomdrawSliderAdamping(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	adamping = nadamping / 100.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSceneDlg::OnNMCustomdrawSliderMu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	if(mu_infinity == FALSE)
	{
		mu = nmu / 100.0;
	}
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSceneDlg::OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	r = nr / 100.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSceneDlg::OnBnClickedCheckInfinity()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(mu_infinity)
	{
		nmu = 100;
		mu = HUGE_VAL;
	}
	else
	{
		nmu = 0;
		mu = 0;
	}
	UpdateData(FALSE);
}
