/*
对话框类
用于设置物体的各种属性
父类CSpecifyDlg通常不被直接调用，
而是用子类CSpecifyBox和CSpecifySphere
父类提供质量、体积、位置、颜色、朝向等公共的属性
子类根据各自的物体特征确定尺寸以及与质量、体积等的关系
*/

#pragma once

#include "Resource.h"
#include "globals.h"

// CSpecifyDlg 对话框

class CSpecifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpecifyDlg)

public:
	CSpecifyDlg(UINT uIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	CSpecifyDlg(MyObject* object, UINT uIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpecifyDlg();

	// 对话框数据

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	void ViewColor();

public:
	// 颜色
	int ncolorR;
	int ncolorG;
	int ncolorB;
	int ncolorAlpha;
	// 位置
	dReal posX;
	dReal posY;
	dReal posZ;
	int nposX;
	int nposY;
	int nposZ;
	// 质量
	dMass Mass;
	dReal massVal;
	int nmass;
	// 密度
	dReal density;
	int ndensity;
	// 方向
	int deg_rad; // 0=deg; 1=rad;
	dMatrix3 R;
	dQuaternion Q;
	dReal AAAx;
	dReal AAAy;
	dReal AAAz;
	dReal AAAs;
protected:
	// 体积
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
	CSpecifySphere(CWnd* pParent = NULL);   // 标准构造函数
	CSpecifySphere(MyObject* object, CWnd* pParent = NULL);   // 标准构造函数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 半径
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
	CSpecifyBox(CWnd* pParent = NULL);   // 标准构造函数
	CSpecifyBox(MyObject* object, CWnd* pParent = NULL);   // 标准构造函数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

