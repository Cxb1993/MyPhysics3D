// MyPhysics.h : MyPhysics Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMyPhysicsApp:
// �йش����ʵ�֣������ MyPhysics.cpp
//

class CMyPhysicsApp : public CWinApp
{
public:
	CMyPhysicsApp();


	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyPhysicsApp theApp;