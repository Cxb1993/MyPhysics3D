// DynamicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPhysics.h"
#include "DynamicDlg.h"


// CDynamicDlg 对话框

IMPLEMENT_DYNAMIC(CDynamicDlg, CDialog)

CDynamicDlg::CDynamicDlg(const MyObject* object, CWnd* pParent /*=NULL*/)
: CDialog(CDynamicDlg::IDD, pParent)
{
	const dReal* vl = dBodyGetLinearVel(object->body);
	const dReal* va = dBodyGetAngularVel(object->body);
	vlx = vl[0];
	vly = vl[1];
	vlz = vl[2];
	vax = va[0];
	vay = va[1];
	vaz = va[2];
	ilx = 0.0;
	ily = 0.0;
	ilz = 0.0;
	iax = 0.0;
	iay = 0.0;
	iaz = 0.0;
	flx = object->force[0];
	fly = object->force[1];
	flz = object->force[2];
	fax = object->torque[0];
	fay = object->torque[1];
	faz = object->torque[2];
}

CDynamicDlg::~CDynamicDlg()
{
}

void CDynamicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VLX, vlx);
	DDX_Text(pDX, IDC_VLY, vly);
	DDX_Text(pDX, IDC_VLZ, vlz);
	DDX_Text(pDX, IDC_VAX, vax);
	DDX_Text(pDX, IDC_VAY, vay);
	DDX_Text(pDX, IDC_VAZ, vaz);
	DDX_Text(pDX, IDC_ILX, ilx);
	DDX_Text(pDX, IDC_ILY, ily);
	DDX_Text(pDX, IDC_ILZ, ilz);
	DDX_Text(pDX, IDC_IAX, iax);
	DDX_Text(pDX, IDC_IAY, iay);
	DDX_Text(pDX, IDC_IAZ, iaz);
	DDX_Text(pDX, IDC_FLX, flx);
	DDX_Text(pDX, IDC_FLY, fly);
	DDX_Text(pDX, IDC_FLZ, flz);
	DDX_Text(pDX, IDC_FAX, fax);
	DDX_Text(pDX, IDC_FAY, fay);
	DDX_Text(pDX, IDC_FAZ, faz);
}


BEGIN_MESSAGE_MAP(CDynamicDlg, CDialog)
END_MESSAGE_MAP()


// CDynamicDlg 消息处理程序
