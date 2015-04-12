#pragma once
#include "resource.h"
#include "globals.h"

// CDynamicDlg 对话框

class CDynamicDlg : public CDialog
{
	DECLARE_DYNAMIC(CDynamicDlg)

public:
	CDynamicDlg(const MyObject* object, CWnd* pParent = NULL);
	virtual ~CDynamicDlg();

	// 对话框数据
	enum { IDD = IDD_DYNAMIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float vlx;
	float vly;
	float vlz;
	float vax;
	float vay;
	float vaz;
	float ilx;
	float ily;
	float ilz;
	float iax;
	float iay;
	float iaz;
	float flx;
	float fly;
	float flz;
	float fax;
	float fay;
	float faz;
};
