/*
�Ի�����
������������ĸ�������
����CSpecifyDlgͨ������ֱ�ӵ��ã�
����������CSpecifyBox��CSpecifySphere
�����ṩ�����������λ�á���ɫ������ȹ���������
������ݸ��Ե���������ȷ���ߴ��Լ�������������ȵĹ�ϵ
*/

#pragma once

#include "Resource.h"
#include "globals.h"

// CSpecifyDlg �Ի���

class CSpecifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpecifyDlg)

public:
	CSpecifyDlg(UINT uIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	CSpecifyDlg(MyObject* object, UINT uIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpecifyDlg();

	// �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void ViewColor();

public:
	// ��ɫ
	int ncolorR;
	int ncolorG;
	int ncolorB;
	int ncolorAlpha;
	// λ��
	dReal posX;
	dReal posY;
	dReal posZ;
	int nposX;
	int nposY;
	int nposZ;
	// ����
	dMass Mass;
	dReal massVal;
	int nmass;
	// �ܶ�
	dReal density;
	int ndensity;
	// ����
	int deg_rad; // 0=deg; 1=rad;
	dMatrix3 R;
	dQuaternion Q;
	dReal AAAx;
	dReal AAAy;
	dReal AAAz;
	dReal AAAs;
protected:
	// ���
	dReal volume;

public:
	afx_msg void OnEnKillfocusPos();
	afx_msg void OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusAaa();
	afx_msg void OnBnClickedRadioDeg_Rad();
	afx_msg void OnEnKillfocusR();
	afx_msg void OnEnKillfocusQ();
	afx_msg void OnOK();
};


class CSpecifySphere : public CSpecifyDlg
{
public:
	enum { IDD = IDD_DIALOG_SPHERE };
	CSpecifySphere(CWnd* pParent = NULL);   // ��׼���캯��
	CSpecifySphere(MyObject* object, CWnd* pParent = NULL);   // ��׼���캯��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// �뾶
	dReal sizeR;
	int nsizeR;
public:
	afx_msg void OnNMCustomdrawSliderSr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDensity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderMass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusSr();
	afx_msg void OnEnKillfocusDensity();
	afx_msg void OnEnKillfocusMass();
};


class CSpecifyBox : public CSpecifyDlg
{
public:
	enum { IDD = IDD_DIALOG_BOX };
	CSpecifyBox(CWnd* pParent = NULL);   // ��׼���캯��
	CSpecifyBox(MyObject* object, CWnd* pParent = NULL);   // ��׼���캯��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	dReal sizeX;
	dReal sizeY;
	dReal sizeZ;
	int nsizeX;
	int nsizeY;
	int nsizeZ;
public:
	afx_msg void OnNMCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDensity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderMass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusSize();
	afx_msg void OnEnKillfocusDensity();
	afx_msg void OnEnKillfocusMass();
};

