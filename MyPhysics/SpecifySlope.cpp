// specifySlope.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPhysics.h"
#include "specifySlope.h"


// CspecifySlope 对话框

IMPLEMENT_DYNAMIC(CspecifySlope, CDialog)

CspecifySlope::CspecifySlope(CWnd* pParent /*=NULL*/)
: CDialog(CspecifySlope::IDD, pParent)
, vel(0)
, nvel(0)
, up(0)
{
	degree = 15.0;
	radian = degree * DEG_TO_RAD;
	nangle = Round(degree * 100.0 / 180.0);
	l = 2.5;
	nl = Round(l * 10.0);
	w = 2;
	nw = Round(w * 10.0);
	h = 0.02;
	nh = Round(h * 200);
}

CspecifySlope::~CspecifySlope(){}

void CspecifySlope::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, degree);
	DDV_MinMaxFloat(pDX, degree, 0, 180);
	DDX_Text(pDX, IDC_EDIT2, radian);
	DDV_MinMaxFloat(pDX, radian, 0, M_PI);
	DDX_Slider(pDX, IDC_SLIDER1, nangle);
	DDX_Text(pDX, IDC_EDIT3, l);
	DDX_Slider(pDX, IDC_SLIDER2, nl);
	DDX_Text(pDX, IDC_EDIT4, w);
	DDX_Slider(pDX, IDC_SLIDER3, nw);
	DDX_Text(pDX, IDC_EDIT5, h);
	DDX_Slider(pDX, IDC_SLIDER4, nh);
	DDX_Text(pDX, IDC_EDIT_VEL, vel);
	DDX_Slider(pDX, IDC_SLIDER5, nvel);
	DDX_Radio(pDX, IDC_RADIO_UP, up);
}


BEGIN_MESSAGE_MAP(CspecifySlope, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CspecifySlope::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CspecifySlope::OnEnKillfocusEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CspecifySlope::OnNMCustomdrawSlider1)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CspecifySlope::OnEnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CspecifySlope::OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CspecifySlope::OnEnKillfocusEdit5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CspecifySlope::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CspecifySlope::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CspecifySlope::OnNMCustomdrawSlider4)
	ON_EN_KILLFOCUS(IDC_EDIT_VEL, &CspecifySlope::OnEnKillfocusEditVel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CspecifySlope::OnNMCustomdrawSlider5)
END_MESSAGE_MAP()


// CspecifySlope 消息处理程序

void CspecifySlope::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	radian = degree * DEG_TO_RAD;
	nangle = Round(degree * 100.0 / 180.0);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnEnKillfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	degree = radian * RAD_TO_DEG;
	nangle = Round(radian * 100.0 / M_PI);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	degree = nangle * 180.0 / 100.0;
	radian = nangle * M_PI / 100.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CspecifySlope::OnEnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nl = Round(l * 10.0);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnEnKillfocusEdit4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nw = Round(w * 10.0);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnEnKillfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nh = Round(h * 200.0);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	l = nl / 10.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CspecifySlope::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	w = nw / 10.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CspecifySlope::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	h = nh / 200.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CspecifySlope::OnEnKillfocusEditVel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nvel = Round(vel * 100.0 / 2.0);
	UpdateData(FALSE); // output data
}

void CspecifySlope::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	vel = nvel * 2.0 / 100.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}
