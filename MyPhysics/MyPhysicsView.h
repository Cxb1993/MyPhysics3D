// MyPhysicsView.h : CMyPhysicsView 类的接口
//

#pragma once

#include <vector>
#include "MyPhysicsDoc.h"
#include "Resource.h"
#include "globals.h"
#include "afxwin.h"

class CMyPhysicsView : public CFormView
{
protected: // 仅从序列化创建
	CMyPhysicsView();
	DECLARE_DYNCREATE(CMyPhysicsView)

public:
	enum{ IDD = IDD_MYPHYSICS_FORM };

	// 属性
public:
	HGLRC m_hRC; // used for OpenGL draw
	int button,lastx,lasty;	// used for mouse events
	BOOL slope_adjust;
	BOOL moving;
	static float slope_angle;
	static float conveyor_vel;
	static BOOL conveyor_dirup;

	// the current state:
	//    0 = uninitialized
	//    1 = dsSimulationLoop() called
	//    2 = dsDrawFrame() called
	static int current_state;

	// show forces, textures and shadows
	static BOOL bshow_force;
	static BOOL use_textures;		// 1 if textures to be drawn
	static BOOL use_shadows;		// 1 if shadows to be drawn
	static Texture *sky_texture;
	static Texture *ground_texture;
	static Texture *wood_texture;
	static Texture *checkered_texture;
	static Texture *texture[4+1]; // +1 since index 0 is not used

	static float Mu;
	static float Bounce;
	static float G;

	// motion model
	// current camera position and orientation
	static float view_xyz[3];	// position x,y,z
	static float view_hpr[3];	// heading, pitch, roll (degrees)

	// globals used to communicate with rendering thread

	BOOL renderer_pause;	  // 0=run, 1=pause
	BOOL renderer_singlestep;
	int renderer_width;
	int renderer_height;
	dsFunctions *renderer_fn;
	HDC renderer_dc;     // used for OpenGL draw
	//***************************************************************************
	// the current drawing state (for when the user's step function is drawing)

	static float color[4];  // current r,g,b,alpha color
	static int tnum;        // current texture number
	static CString szFileName;     // for saving scn file

	////////////////////////// END OF MY OWN VARIABLES /////////////////////////////////

public:
	CMyPhysicsDoc* GetDocument() const;

	// 操作
public:
	// drawstuff API:
	static void dsSetViewpoint (float xyz[3], float hpr[3]);

	static void dsGetViewpoint (float xyz[3], float hpr[3]);

	static void dsSetTexture (int texture_number);

	static void dsSetColor (float red, float green, float blue);

	static void dsSetColorAlpha (float red, float green, float blue, float alpha);

	static void dsDrawBox (const dReal pos[3], const dReal R[12],
		const dReal sides[3]);

	static void dsDrawSphere (const dReal pos[3], const dReal R[12],
		dReal radius);

	static void dsDrawTriangle (const dReal pos[3], const dReal R[12],
		const dReal *v0, const dReal *v1,
		const dReal *v2, int solid);

	static void dsDrawCylinder (const dReal pos[3], const dReal R[12],
		float length, float radius);

	static void dsDrawCapsule (const dReal pos[3], const dReal R[12],
		float length, float radius);

	static void dsDrawLine (const dReal pos1[3], const dReal pos2[3]);

	static void dsDrawArrow (const dReal pos1[3], const dReal pos2[3]);

	static void dsDrawConvex (const dReal pos[3], const dReal R[12],
		dReal *_planes,unsigned int _planecount,
		dReal *_points,
		unsigned int _pointcount,
		unsigned int *_polygons);

	static void setupDrawingMode();
	static void setShadowDrawingMode();
	static void wrapCameraAngles();
	void initMotionModel();
	void dsMotion (int mode, int deltax, int deltay);
	void dsStartGraphics (int width, int height, dsFunctions *fn);
	void dsStopGraphics();
	void drawSky (float view_xyz[3]);
	void drawGround();
	void drawPyramidGrid();
	void dsDrawFrame (int width, int height, dsFunctions *fn, int pause);

	///////////////////////////////////////////////////////////////////////
	// view simulation:
	static void start();
	static void stop();
	static void simLoop (int pause);
	static void nearCallback (void *data, dGeomID o1, dGeomID o2);
	static void drawGeom (dGeomID g, const dReal *pos, const dReal *R);

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

	// 实现
public:
	virtual ~CMyPhysicsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnSimulation();
	afx_msg void OnBox();
	afx_msg void OnSphere();
	afx_msg void OnSlope();
	afx_msg void OnDelete();
	afx_msg void OnSetStatic();
	afx_msg void OnSetDynamic();

	afx_msg void OnUpdateData();
	afx_msg void OnBnClickedStep();


	//{{AFX_MSG(CMyPhysicsView)
protected:
	DECLARE_MESSAGE_MAP()

	////////////////////////////////PLACE HERE TEMPORARILY////////////////////////////////

public:
	// dynamics and collision objects

	static int num;		// number of objects in simulation
	static int nextobj;		// next object to recycle if num==NUM
	static dWorldID world;
	static dSpaceID space;
	static MyObject obj[NUM];
	static dJointGroupID contactgroup; // the contact group ID
	static int selected;	// selected object
	static int write_world; // for file saving
	static dGeomID ground_box; // the slope ID
	static dGeomID select_ray; // the select ray ID
	static BOOL select_finish; // is selection finished?

	//static MyFeedback feedbacks[MAX_FEEDBACKNUM];
	static std::vector<MyFeedback> feedbackvector;
	static size_t fbnum;
	////////////////////////////////PLACE HERE TEMPORARILY////////////////////////////////

public:
	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);

public:
	afx_msg void OnEnKillfocusScale();
public:
	static float ForceScale;
	int nForceScale;
public:
	afx_msg void OnNMCustomdrawSliderScale(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnSpecifyScene();
public:
	afx_msg void OnSelectCancel();
public:
	afx_msg void OnUpdateSelect(CCmdUI *pCmdUI);
public:
	afx_msg void OnSelectNext();
public:
	afx_msg void OnBnClickedPlayPause();
public:
	CButton BNpause;
};

#ifndef _DEBUG  // MyPhysicsView.cpp 中的调试版本
inline CMyPhysicsDoc* CMyPhysicsView::GetDocument() const
{ return reinterpret_cast<CMyPhysicsDoc*>(m_pDocument); }
#endif

