#pragma once
#include "resource.h"

// CSceneDlg �Ի���

class CSceneDlg : public CDialog
{
	DECLARE_DYNAMIC(CSceneDlg)

public:
	CSceneDlg(float G,float Ldamping, float Adamping, float Mu, float Bounce, CWnd* pParent = NULL);
	virtual ~CSceneDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_WORLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEditG();
	afx_msg void OnEnKillfocusEditLdamping();
	afx_msg void OnEnKillfocusEditAdamping();
	afx_msg void OnEnKillfocusEditMu();
	afx_msg void OnEnKillfocusEditR();
	afx_msg void OnNMCustomdrawSliderG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLdamping(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderAdamping(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderMu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult);
public:
	float g;
	int ng;
	float ldamping;
	int nldamping;
	float adamping;
	int nadamping;
	BOOL mu_infinity;
	float mu;
	int nmu;
	float r;
	int nr;
public:
	afx_msg void OnBnClickedCheckInfinity();

};
