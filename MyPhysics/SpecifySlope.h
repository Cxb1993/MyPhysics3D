#pragma once

#include "Resource.h"
#include "globals.h"

// CspecifySlope 对话框

class CspecifySlope : public CDialog
{
	DECLARE_DYNAMIC(CspecifySlope)

public:
	CspecifySlope(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CspecifySlope();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SLOPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float degree;
	float radian;
	int nangle;
	float l;
	int nl;
	float w;
	int nw;
	float h;
	int nh;
	float vel;
	int nvel;
	int up;
public:
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit5();
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditVel();
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
};
