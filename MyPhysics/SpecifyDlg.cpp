// SpecifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SpecifyDlg.h"


// CSpecifyDlg 对话框

IMPLEMENT_DYNAMIC(CSpecifyDlg, CDialog)

CSpecifyDlg::CSpecifyDlg(UINT uIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(uIDTemplate, pParent)
{
	posX = 0;
	posY = 0;
	posZ = 0;
	ncolorR = dRandInt(101);
	ncolorG = dRandInt(101);
	ncolorB = dRandInt(101);
	ncolorAlpha = 75;
	nposX = Round(posX * 10.0 + 50.0);
	nposY = Round(posY * 10.0 + 50.0);
	nposZ = Round(posZ * 10.0);

	dMassSetZero(&Mass);
	density = 5.0;
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	ndensity = Round(density * 10.0);

	dRSetIdentity(R);
	dQSetIdentity(Q);

	deg_rad = 0; // default to degree
	AAAx = 0.0;
	AAAy = 0.0;
	AAAz = 1.0;
	AAAs = 0.0;
}

CSpecifyDlg::CSpecifyDlg(MyObject* object,UINT uIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(uIDTemplate, pParent)
{
	const dReal* pos = dBodyGetPosition(object->body);
	posX = pos[0];
	posY = pos[1];
	posZ = pos[2];
	nposX = Round(posX * 10.0 + 50.0);
	nposY = Round(posY * 10.0 + 50.0);
	nposZ = Round(posZ * 10.0);

	ncolorR = Round(object->color[0]*100.0);
	ncolorG = Round(object->color[1]*100.0);
	ncolorB = Round(object->color[2]*100.0);
	ncolorAlpha = Round(object->color[3]*100.0);

	const dReal* Rot = dBodyGetRotation(object->body);
	const dReal* Qua = dBodyGetQuaternion(object->body);
	for(int i=0; i<12; i++)
	{
		R[i] = Rot[i];
	}
	for(int i=0; i<4; i++)
	{
		Q[i] = Qua[i];
	}
	deg_rad = 0; // default to degree

	dReal halfsita; // sita/2
	halfsita = acos(Q[0]);
	dReal sinhalfsita = sin(halfsita);
	if(sinhalfsita == 0)
	{
		AAAx = 0.0;
		AAAy = 0.0;
		AAAz = 1.0;
		AAAs = 0.0;
	}
	else
	{
		AAAx = Q[1] / sinhalfsita;
		AAAy = Q[2] / sinhalfsita;
		AAAz = Q[3] / sinhalfsita;
		AAAs = halfsita * 2.0;
		if(deg_rad == 0) AAAs *= RAD_TO_DEG;
	}
}

CSpecifyDlg::~CSpecifyDlg()
{
}

void CSpecifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 位置
	DDX_Slider(pDX, IDC_SLIDER_PX, nposX);
	DDX_Slider(pDX, IDC_SLIDER_PY, nposY);
	DDX_Slider(pDX, IDC_SLIDER_PZ, nposZ);
	DDX_Text(pDX, IDC_POSX, posX);
	DDX_Text(pDX, IDC_POSY, posY);
	DDX_Text(pDX, IDC_POSZ, posZ);
	// 颜色
	DDX_Slider(pDX, IDC_SLIDER_R, ncolorR);
	DDX_Slider(pDX, IDC_SLIDER_G, ncolorG);
	DDX_Slider(pDX, IDC_SLIDER_B, ncolorB);
	DDX_Slider(pDX, IDC_SLIDER_ALPHA, ncolorAlpha);
	// 质量
	DDX_Text(pDX, IDC_MASS, massVal);
	DDX_Slider(pDX, IDC_SLIDER_MASS, nmass);
	// 密度
	DDX_Text(pDX, IDC_DENSITY, density);
	DDX_Slider(pDX, IDC_SLIDER_DENSITY, ndensity);
	// 方向
	DDX_Radio(pDX, IDC_RADIO_DEG, deg_rad);

	DDX_Text(pDX, IDC_AAAX, AAAx);
	DDX_Text(pDX, IDC_AAAY, AAAy);
	DDX_Text(pDX, IDC_AAAZ, AAAz);
	DDX_Text(pDX, IDC_AAAS, AAAs);

	DDX_Text(pDX, IDC_R00, R[0*4+0]);
	DDX_Text(pDX, IDC_R01, R[0*4+1]);
	DDX_Text(pDX, IDC_R02, R[0*4+2]);
	DDX_Text(pDX, IDC_R10, R[1*4+0]);
	DDX_Text(pDX, IDC_R11, R[1*4+1]);
	DDX_Text(pDX, IDC_R12, R[1*4+2]);
	DDX_Text(pDX, IDC_R20, R[2*4+0]);
	DDX_Text(pDX, IDC_R21, R[2*4+1]);
	DDX_Text(pDX, IDC_R22, R[2*4+2]);

	DDX_Text(pDX, IDC_Q0, Q[0]);
	DDX_Text(pDX, IDC_Q1, Q[1]);
	DDX_Text(pDX, IDC_Q2, Q[2]);
	DDX_Text(pDX, IDC_Q3, Q[3]);
}

BEGIN_MESSAGE_MAP(CSpecifyDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_R, &CSpecifyDlg::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_G, &CSpecifyDlg::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_B, &CSpecifyDlg::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CSpecifyDlg::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PX, &CSpecifyDlg::OnNMCustomdrawSliderPos)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PY, &CSpecifyDlg::OnNMCustomdrawSliderPos)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PZ, &CSpecifyDlg::OnNMCustomdrawSliderPos)
	ON_EN_KILLFOCUS(IDC_POSX, &CSpecifyDlg::OnEnKillfocusPos)
	ON_EN_KILLFOCUS(IDC_POSY, &CSpecifyDlg::OnEnKillfocusPos)
	ON_EN_KILLFOCUS(IDC_POSZ, &CSpecifyDlg::OnEnKillfocusPos)
	ON_BN_CLICKED(IDC_RADIO_DEG, &CSpecifyDlg::OnBnClickedRadioDeg_Rad)
	ON_BN_CLICKED(IDC_RADIO_RAD, &CSpecifyDlg::OnBnClickedRadioDeg_Rad)
	ON_EN_KILLFOCUS(IDC_AAAX, &CSpecifyDlg::OnEnKillfocusAaa)
	ON_EN_KILLFOCUS(IDC_AAAY, &CSpecifyDlg::OnEnKillfocusAaa)
	ON_EN_KILLFOCUS(IDC_AAAZ, &CSpecifyDlg::OnEnKillfocusAaa)
	ON_EN_KILLFOCUS(IDC_AAAS, &CSpecifyDlg::OnEnKillfocusAaa)
	ON_EN_KILLFOCUS(IDC_R00, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R01, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R02, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R10, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R11, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R12, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R20, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R21, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_R22, &CSpecifyDlg::OnEnKillfocusR)
	ON_EN_KILLFOCUS(IDC_Q0, &CSpecifyDlg::OnEnKillfocusQ)
	ON_EN_KILLFOCUS(IDC_Q1, &CSpecifyDlg::OnEnKillfocusQ)
	ON_EN_KILLFOCUS(IDC_Q2, &CSpecifyDlg::OnEnKillfocusQ)
	ON_EN_KILLFOCUS(IDC_Q3, &CSpecifyDlg::OnEnKillfocusQ)
END_MESSAGE_MAP()

// CSpecifyDlg 消息处理程序

void CSpecifyDlg::OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ViewColor();
	*pResult = 0;
}

void CSpecifyDlg::ViewColor()
{
	RECT rect;
	GetClientRect(&rect);
	rect.bottom -= 115;
	rect.top = rect.bottom-140;
	rect.right -= 85;
	rect.left = rect.right-160;
	CDC* pDC = GetDC();
	CBrush brush1(RGB(
		(100-ncolorAlpha)*255/100+ncolorR*ncolorAlpha*255/10000,
		(100-ncolorAlpha)*255/100+ncolorG*ncolorAlpha*255/10000,
		(100-ncolorAlpha)*255/100+ncolorB*ncolorAlpha*255/10000));
	CBrush brush2(RGB(
		ncolorR*ncolorAlpha*255/10000,
		ncolorG*ncolorAlpha*255/10000,
		ncolorB*ncolorAlpha*255/10000));
	RECT subrc;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			subrc.left = rect.left + i * (rect.right-rect.left) / 4;
			subrc.right = subrc.left + (rect.right-rect.left) / 4;
			subrc.top = rect.top + j * (rect.bottom-rect.top) / 4;
			subrc.bottom = subrc.top + (rect.bottom-rect.top) / 4;
			if((i+j)%2 == 0)
				pDC->FillRect(&subrc, &brush1);
			else
				pDC->FillRect(&subrc, &brush2);
		}
	}
}

void CSpecifyDlg::OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	posX = (nposX-50)/10.0;
	posY = (nposY-50)/10.0;
	posZ = nposZ/10.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifyDlg::OnEnKillfocusPos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nposX = Round(posX * 10 + 50);
	nposY = Round(posY * 10 + 50);
	nposZ = Round(posZ * 10);
	UpdateData(FALSE); // output data
}

void CSpecifyDlg::OnBnClickedRadioDeg_Rad()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	if(deg_rad == 0) AAAs *= RAD_TO_DEG;
	else AAAs *= DEG_TO_RAD;
	UpdateData(FALSE); // output data
}

void CSpecifyDlg::OnEnKillfocusAaa()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	dRFromAxisAndAngle(R, AAAx, AAAy, AAAz, deg_rad ? AAAs : AAAs*DEG_TO_RAD);
	dQFromAxisAndAngle(Q, AAAx, AAAy, AAAz, deg_rad ? AAAs : AAAs*DEG_TO_RAD);
	UpdateData(FALSE); // output data
}

void CSpecifyDlg::OnEnKillfocusR()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	dQfromR(Q,R);
	dReal halfsita; // sita/2
	halfsita = acos(Q[0]);
	dReal sinhalfsita = sin(halfsita);
	if(sinhalfsita == 0)
	{
		AAAx = 0.0;
		AAAy = 0.0;
		AAAz = 1.0;
		AAAs = 0.0;
	}
	else
	{
		AAAx = Q[1] / sinhalfsita;
		AAAy = Q[2] / sinhalfsita;
		AAAz = Q[3] / sinhalfsita;
		AAAs = halfsita * 2.0;
		if(deg_rad == 0) AAAs *= RAD_TO_DEG;
	}
	UpdateData(FALSE); // output data
}

void CSpecifyDlg::OnEnKillfocusQ()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	dRfromQ(R,Q);
	dReal halfsita; // sita/2
	halfsita = acos(Q[0]);
	dReal sinhalfsita = sin(halfsita);
	if(sinhalfsita == 0)
	{
		AAAx = 0.0;
		AAAy = 0.0;
		AAAz = 1.0;
		AAAs = 0.0;
	}
	else
	{
		AAAx = Q[1] / sinhalfsita;
		AAAy = Q[2] / sinhalfsita;
		AAAz = Q[3] / sinhalfsita;
		AAAs = halfsita * 2.0;
		if(deg_rad == 0) AAAs *= RAD_TO_DEG;
	}
	UpdateData(FALSE); // output data
}

void CSpecifyDlg::OnOK()
{
	if(Mass.mass == 0)
	{
		MessageBox(_T("质量不能为零！"),0,MB_OK|MB_ICONWARNING);
		return;
	}
	if(volume == 0)
	{
		MessageBox(_T("体积不能为零！"),0,MB_OK|MB_ICONWARNING);
		return;
	}
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////

CSpecifySphere::CSpecifySphere(CWnd* pParent /*=NULL*/)
: CSpecifyDlg(IDD, pParent)
{
	sizeR = 0.2;
	nsizeR = Round(sizeR * 100.0 / 2.0);
	volume = 4.0 / 3.0 * M_PI * sizeR * sizeR * sizeR;
	dMassSetSphere(&Mass,density,sizeR);
	nmass = Round(Mass.mass * 100.0 / 5.0);
}

CSpecifySphere::CSpecifySphere(MyObject* object, CWnd* pParent /*=NULL*/)
: CSpecifyDlg(object, IDD, pParent)
{
	sizeR = dGeomSphereGetRadius(object->geom);
	nsizeR = Round(sizeR * 100.0 / 2.0);
	volume = 4.0 / 3.0 * M_PI * sizeR * sizeR * sizeR;
	dBodyGetMass(object->body, &Mass);
	density = Mass.mass / volume;
	ndensity = Round(density * 10);
	nmass = Round(Mass.mass * 100 / 5);
}

void CSpecifySphere::DoDataExchange(CDataExchange* pDX)
{
	CSpecifyDlg::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_SR, nsizeR);
	DDX_Text(pDX, IDC_SR, sizeR);
}

BEGIN_MESSAGE_MAP(CSpecifySphere, CSpecifyDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SR, &CSpecifySphere::OnNMCustomdrawSliderSr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MASS, &CSpecifySphere::OnNMCustomdrawSliderMass)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DENSITY, &CSpecifySphere::OnNMCustomdrawSliderDensity)
	ON_EN_KILLFOCUS(IDC_SR, &CSpecifySphere::OnEnKillfocusSr)
	ON_EN_KILLFOCUS(IDC_DENSITY, &CSpecifySphere::OnEnKillfocusDensity)
	ON_EN_KILLFOCUS(IDC_MASS, &CSpecifySphere::OnEnKillfocusMass)
END_MESSAGE_MAP()


void CSpecifySphere::OnNMCustomdrawSliderSr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	sizeR = nsizeR / 100.0 * 2.0;
	dMassSetSphere(&Mass, density, sizeR);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	volume = 4.0 / 3.0 * M_PI * sizeR * sizeR * sizeR;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifySphere::OnNMCustomdrawSliderDensity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	density = ndensity / 10.0;
	dMassSetSphere(&Mass, density, sizeR);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifySphere::OnNMCustomdrawSliderMass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	massVal = nmass / 100.0 * 5.0;
	dMassSetSphereTotal(&Mass, massVal, sizeR);
	density = massVal / volume;
	ndensity = Round(density * 10.0);
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifySphere::OnEnKillfocusSr()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nsizeR = Round(sizeR * 20.0);
	dMassSetSphere(&Mass, density, sizeR);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	volume = 4.0 / 3.0 * M_PI * sizeR * sizeR * sizeR;
	UpdateData(FALSE); // output data
}

void CSpecifySphere::OnEnKillfocusDensity()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ndensity = Round(density * 10.0);
	dMassSetSphere(&Mass, density, sizeR);
	massVal = Mass.mass;
	nmass = Round(Mass.mass * 100.0 / 5.0);
	UpdateData(FALSE); // output data
}

void CSpecifySphere::OnEnKillfocusMass()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	dMassSetSphereTotal(&Mass, massVal, sizeR);
	nmass = Round(massVal * 100.0 / 5.0);
	density = massVal / volume;
	ndensity = Round(density * 10.0);
	UpdateData(FALSE); // output data
}
/////////////////////////////////////////////////////////

CSpecifyBox::CSpecifyBox(CWnd* pParent /*=NULL*/)
: CSpecifyDlg(IDD, pParent)
{
	sizeX = 0.3;
	sizeY = 0.3;
	sizeZ = 0.3;
	nsizeX = Round(sizeX * 100.0 / 3.0);
	nsizeY = Round(sizeY * 100.0 / 3.0);
	nsizeZ = Round(sizeZ * 100.0 / 3.0);
	volume = sizeX * sizeY * sizeZ;
	dMassSetBox(&Mass, density, sizeX, sizeY, sizeZ);
	nmass = Round(Mass.mass * 100.0 / 5.0);
}

CSpecifyBox::CSpecifyBox(MyObject* object, CWnd* pParent /*=NULL*/)
: CSpecifyDlg(object, IDD, pParent)
{
	dVector3 result;
	dGeomBoxGetLengths(object->geom, result);
	sizeX = result[0];
	sizeY = result[1];
	sizeZ = result[2];
	nsizeX = Round(sizeX * 100.0 / 3.0);
	nsizeY = Round(sizeY * 100.0 / 3.0);
	nsizeZ = Round(sizeZ * 100.0 / 3.0);
	volume = sizeX * sizeY * sizeZ;
	dBodyGetMass(object->body, &Mass);
	massVal = Mass.mass;
	density = massVal / volume;
	ndensity = Round(density * 10.0);
	nmass = Round(massVal * 100.0 / 5.0);
}
void CSpecifyBox::DoDataExchange(CDataExchange* pDX)
{
	CSpecifyDlg::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_SX, nsizeX);
	DDX_Slider(pDX, IDC_SLIDER_SY, nsizeY);
	DDX_Slider(pDX, IDC_SLIDER_SZ, nsizeZ);
	DDX_Text(pDX, IDC_SX, sizeX);
	DDX_Text(pDX, IDC_SY, sizeY);
	DDX_Text(pDX, IDC_SZ, sizeZ);
}

BEGIN_MESSAGE_MAP(CSpecifyBox, CSpecifyDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SX, &CSpecifyBox::OnNMCustomdrawSliderSize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SY, &CSpecifyBox::OnNMCustomdrawSliderSize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SZ, &CSpecifyBox::OnNMCustomdrawSliderSize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MASS, &CSpecifyBox::OnNMCustomdrawSliderMass)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DENSITY, &CSpecifyBox::OnNMCustomdrawSliderDensity)
	ON_EN_KILLFOCUS(IDC_SX, &CSpecifyBox::OnEnKillfocusSize)
	ON_EN_KILLFOCUS(IDC_SY, &CSpecifyBox::OnEnKillfocusSize)
	ON_EN_KILLFOCUS(IDC_SZ, &CSpecifyBox::OnEnKillfocusSize)
	ON_EN_KILLFOCUS(IDC_DENSITY, &CSpecifyBox::OnEnKillfocusDensity)
	ON_EN_KILLFOCUS(IDC_MASS, &CSpecifyBox::OnEnKillfocusMass)
END_MESSAGE_MAP()


void CSpecifyBox::OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	sizeX = nsizeX / 100.0 * 3.0;
	sizeY = nsizeY / 100.0 * 3.0;
	sizeZ = nsizeZ / 100.0 * 3.0;
	dMassSetBox(&Mass, density, sizeX, sizeY, sizeZ);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	volume = sizeX * sizeY * sizeZ;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifyBox::OnNMCustomdrawSliderDensity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	density = ndensity / 10.0;
	dMassSetBox(&Mass, density, sizeX, sizeY, sizeZ);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifyBox::OnNMCustomdrawSliderMass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	massVal = nmass / 100.0 * 5.0;
	dMassSetBoxTotal(&Mass, massVal, sizeX, sizeY, sizeZ);
	density = massVal / volume;
	ndensity = Round(density * 10.0);
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CSpecifyBox::OnEnKillfocusSize()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	nsizeX = Round(sizeX * 100.0 / 3.0);
	nsizeY = Round(sizeY * 100.0 / 3.0);
	nsizeZ = Round(sizeZ * 100.0 / 3.0);
	dMassSetBox(&Mass, density, sizeX, sizeY, sizeZ);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	volume = sizeX * sizeY * sizeZ;
	UpdateData(FALSE); // output data
}

void CSpecifyBox::OnEnKillfocusDensity()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ndensity = Round(density * 10.0);
	dMassSetBox(&Mass, density, sizeX, sizeY, sizeZ);
	massVal = Mass.mass;
	nmass = Round(massVal * 100.0 / 5.0);
	UpdateData(FALSE); // output data
}

void CSpecifyBox::OnEnKillfocusMass()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	dMassSetBoxTotal(&Mass, massVal, sizeX, sizeY, sizeZ);
	nmass = Round(massVal * 100.0 / 5.0);
	density = massVal / volume;
	ndensity = Round(density * 10.0);
	UpdateData(FALSE); // output data
}
