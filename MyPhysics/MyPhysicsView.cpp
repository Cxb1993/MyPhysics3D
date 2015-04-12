// MyPhysicsView.cpp : CMyPhysicsView 类的实现
//

#include "stdafx.h"

#include "MyPhysicsView.h"
#include "SceneDlg.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyPhysicsView

IMPLEMENT_DYNCREATE(CMyPhysicsView, CFormView)

BEGIN_MESSAGE_MAP(CMyPhysicsView, CFormView)
	// View Messages
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	// Mouse Operations
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	// Tool bar commands
	ON_COMMAND(ID_BOX, &CMyPhysicsView::OnBox)
	ON_COMMAND(ID_SPHERE, &CMyPhysicsView::OnSphere)
	ON_COMMAND(ID_SLOPE, &CMyPhysicsView::OnSlope)
	ON_COMMAND(ID_DELETE, &CMyPhysicsView::OnDelete)
	// Body set
	ON_COMMAND(ID_SET_STATIC, &CMyPhysicsView::OnSetStatic)
	ON_COMMAND(ID_SET_DYNAMIC, &CMyPhysicsView::OnSetDynamic)
	// Check updates
	ON_BN_CLICKED(IDC_CHECK_SHADOW, &CMyPhysicsView::OnUpdateData)
	ON_BN_CLICKED(IDC_CHECK_TEXTURE, &CMyPhysicsView::OnUpdateData)
	ON_BN_CLICKED(IDC_CHECK_FORCE, &CMyPhysicsView::OnUpdateData)
	// New scene
	ON_COMMAND(ID_FILE_NEW, &CMyPhysicsView::OnFileNew)
	// Single step
	ON_BN_CLICKED(IDC_STEP, &CMyPhysicsView::OnBnClickedStep)
	// File save
	ON_COMMAND(ID_FILE_SAVE, &CMyPhysicsView::OnFileSave)
	// Force scale
	ON_EN_KILLFOCUS(IDC_SCALE, &CMyPhysicsView::OnEnKillfocusScale)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCALE, &CMyPhysicsView::OnNMCustomdrawSliderScale)
	// Scene specification
	ON_COMMAND(ID_SPECIFY_SCENE, &CMyPhysicsView::OnSpecifyScene)
	// Select cancel
	ON_COMMAND(ID_SELECT_CANCEL, &CMyPhysicsView::OnSelectCancel)
	// Select next
	ON_COMMAND(ID_SELECT_NEXT, &CMyPhysicsView::OnSelectNext)
	// Update selecting
	ON_UPDATE_COMMAND_UI(ID_SELECT_CANCEL, &CMyPhysicsView::OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CMyPhysicsView::OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(ID_SET_STATIC, &CMyPhysicsView::OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(ID_SET_DYNAMIC, &CMyPhysicsView::OnUpdateSelect)
	// Play & Pause button
	ON_BN_CLICKED(IDC_PLAY_PAUSE, &CMyPhysicsView::OnBnClickedPlayPause)
END_MESSAGE_MAP()

////////////////////////////////PLACE HERE TEMPORARILY////////////////////////////////
// static variables defination

int CMyPhysicsView::current_state = 0;
int CMyPhysicsView::tnum = 0;
float CMyPhysicsView::view_xyz[3];
float CMyPhysicsView::view_hpr[3];
float CMyPhysicsView::color[4];
BOOL CMyPhysicsView::use_textures = TRUE;       // 1 if textures to be drawn
BOOL CMyPhysicsView::use_shadows = TRUE;        // 1 if shadows to be drawn	
BOOL CMyPhysicsView::bshow_force = FALSE;       // 1 if forces to be drawn

// textures and shadows

Texture* CMyPhysicsView::sky_texture = 0;
Texture* CMyPhysicsView::ground_texture = 0;
Texture* CMyPhysicsView::wood_texture = 0;
Texture* CMyPhysicsView::checkered_texture = 0;
Texture* CMyPhysicsView::texture[4+1];

CString CMyPhysicsView::szFileName;     // for saving scn file
//////////////////////////////////////////////////////////////////

int CMyPhysicsView::num=0;		// number of objects in simulation
int CMyPhysicsView::nextobj=0;		// next object to recycle if num==NUM
dWorldID CMyPhysicsView::world;
dSpaceID CMyPhysicsView::space;
MyObject CMyPhysicsView::obj[NUM];
dJointGroupID CMyPhysicsView::contactgroup;
int CMyPhysicsView::selected = -1;	// selected object
int CMyPhysicsView::write_world = 0;
dGeomID CMyPhysicsView::ground_box = 0;
float CMyPhysicsView::slope_angle = 0.15;
float CMyPhysicsView::conveyor_vel = 0.2;
BOOL CMyPhysicsView::conveyor_dirup = TRUE;

dGeomID CMyPhysicsView::select_ray = 0;
BOOL CMyPhysicsView::select_finish = FALSE; // is selection finished?

std::vector<MyFeedback> CMyPhysicsView::feedbackvector;
size_t CMyPhysicsView::fbnum;
float CMyPhysicsView::ForceScale = 1.0;
float CMyPhysicsView::Mu = dInfinity;
float CMyPhysicsView::Bounce = 0.3;
float CMyPhysicsView::G = 2.5;
////////////////////////////////PLACE HERE TEMPORARILY////////////////////////////////

// CMyPhysicsView 构造/析构

CMyPhysicsView::CMyPhysicsView()
: CFormView(CMyPhysicsView::IDD)
, nForceScale(100-ForceScale*20)
{
	// TODO: 在此处添加构造代码
	slope_adjust = FALSE;
	moving = FALSE;

	button = 0;
	lastx = 0;
	lasty = 0;
	use_shadows = TRUE;
	use_textures = TRUE;
	bshow_force = FALSE;

	renderer_fn = new dsFunctions;
	renderer_fn->version = DS_VERSION;
	renderer_fn->start = &CMyPhysicsView::start;
	renderer_fn->stop = &CMyPhysicsView::stop;
	renderer_fn->step = &CMyPhysicsView::simLoop;
	renderer_fn->command = 0;
	renderer_fn->path_to_textures = 0;	// uses default

	// static variables used to communicate with simloop

	renderer_pause = FALSE;	  // 0=run, 1=pause
	renderer_singlestep = FALSE;
	renderer_width = 1;   // adjust them later
	renderer_height = 1;

	///////////////////////////////////////////////////////
	// create world
	dInitODE2(0);
	world = dWorldCreate();
	space = dHashSpaceCreate (0);
	contactgroup = dJointGroupCreate (0);
	dWorldSetGravity (world,0,0,-G);
	dWorldSetCFM (world,1e-5);
	dWorldSetMaxAngularSpeed(world, 50);
	dWorldSetContactMaxCorrectingVel (world,0.3);
	dWorldSetContactSurfaceLayer (world,0.001);
	dCreatePlane (space,0,0,1,0);
	memset (obj,0,sizeof(obj));
}

CMyPhysicsView::~CMyPhysicsView()
{
	dJointGroupDestroy (contactgroup);
	dSpaceDestroy (space);
	dWorldDestroy (world);
	dCloseODE();
	delete renderer_fn;
}

void CMyPhysicsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_SHADOW, use_shadows);
	DDX_Check(pDX, IDC_CHECK_TEXTURE, use_textures);
	DDX_Check(pDX, IDC_CHECK_FORCE, bshow_force);
	DDX_Text(pDX, IDC_SCALE, ForceScale);
	DDV_MinMaxFloat(pDX, ForceScale,0,5);
	DDX_Slider(pDX, IDC_SLIDER_SCALE, nForceScale);
	DDX_Control(pDX, IDC_PLAY_PAUSE, BNpause);
}

BOOL CMyPhysicsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CFormView::PreCreateWindow(cs);
}

void CMyPhysicsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CMyPhysicsView 诊断

#ifdef _DEBUG
void CMyPhysicsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyPhysicsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMyPhysicsDoc* CMyPhysicsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyPhysicsDoc)));
	return (CMyPhysicsDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyPhysicsView 消息处理程序

int CMyPhysicsView::OnCreate(LPCREATESTRUCT lpcs)
{
	if(CView::OnCreate(lpcs) == -1) return -1;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
		1,				     // version number
		PFD_DRAW_TO_WINDOW |	     // support window
		PFD_SUPPORT_OPENGL |	     // support OpenGL
		PFD_DOUBLEBUFFER,		     // double buffered
		PFD_TYPE_RGBA,		     // RGBA type
		24, 			     // 24-bit color depth
		0, 0, 0, 0, 0, 0,		     // color bits ignored
		0,				     // no alpha buffer
		0,				     // shift bit ignored
		0,				     // no accumulation buffer
		0, 0, 0, 0, 		     // accum bits ignored
		32, 			     // 32-bit z-buffer
		0,				     // no stencil buffer
		0,				     // no auxiliary buffer
		PFD_MAIN_PLANE,		     // main layer
		0,				     // reserved
		0, 0, 0			     // layer masks ignored
	};
	renderer_dc = GetDC()->GetSafeHdc();
	// get the best available match of pixel format for the device context
	int iPixelFormat = ChoosePixelFormat (renderer_dc,&pfd);
	if (iPixelFormat==0) ASSERT("could not find a good OpenGL pixel format");
	// set the pixel format of the device context
	if (SetPixelFormat (renderer_dc,iPixelFormat,&pfd)==FALSE)
		ASSERT("could not set DC pixel format for OpenGL");
	m_hRC = wglCreateContext(renderer_dc);
	if (m_hRC==NULL)
		ASSERT("could not create OpenGL context");
	if (wglMakeCurrent (renderer_dc,m_hRC) != TRUE)
		ASSERT("could not make OpenGL context current");

	//////////////////////////dsSimulationLoop/////////////////////////////////

	if (current_state != 0) ASSERT ("dsSimulationLoop() called more than once");
	current_state = 1;
	if (renderer_fn->version > DS_VERSION)
		ASSERT ("bad version number in dsFunctions structure");

	initMotionModel();

	// test openGL capabilities
	int maxtsize=0;
	glGetIntegerv (GL_MAX_TEXTURE_SIZE,&maxtsize);
	if (maxtsize < 128) ASSERT ("max texture size too small");

	dsStartGraphics (renderer_width,renderer_height,renderer_fn);
	if (renderer_fn->start) renderer_fn->start();

	SetTimer(1,10,NULL);

	return 0;
}

void CMyPhysicsView::OnDestroy()
{
	KillTimer(1);
	dsStopGraphics();
	CView::OnDestroy();

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(m_hRC);
}

void CMyPhysicsView::OnTimer(UINT_PTR nIDEvent)
{
	dsDrawFrame (renderer_width,renderer_height,renderer_fn,renderer_pause && !renderer_singlestep);
	SwapBuffers (renderer_dc);
	if(renderer_singlestep) renderer_singlestep = FALSE;
}

void CMyPhysicsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType,cx,cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30,1,3,3);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,cx,cy);
	glShadeModel(GL_FLAT);
	// adjust the OpenGL picture to the subview
	RECT now_rect;
	GetClientRect(&now_rect);
	renderer_width = now_rect.right-now_rect.left;
	renderer_height = now_rect.bottom-now_rect.top;
}

void CMyPhysicsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(nFlags & MK_SHIFT) // Shift + LButton : selecting a body
	{
		int mouseX = point.x;
		int mouseY = point.y;
		GLint viewport[4];
		GLdouble mvMatrix[16],projMatrix[16];
		glGetIntegerv (GL_VIEWPORT,viewport);
		glGetDoublev (GL_MODELVIEW_MATRIX,mvMatrix);
		glGetDoublev (GL_PROJECTION_MATRIX,projMatrix);
		//获得观察流水线上各种矩阵
		int realY=viewport[3]-mouseY;
		//Windows窗口坐标默认顶点在左上角，这里转化为Opengl默认的左下角

		double tempX,tempY,tempZ;
		gluUnProject ((double) mouseX,(double) realY,0.0,
			mvMatrix,projMatrix,viewport,
			&tempX,&tempY,&tempZ);
		dVector3 point1;
		point1[0]=tempX;
		point1[1]=tempY;
		point1[2]=tempZ;
		//获取x,realY在近侧裁剪平面处对应的物体坐标系坐标（反转到棋盘绘制时的坐标系）

		gluUnProject ((double) mouseX,(double) realY,1.0,
			mvMatrix,projMatrix,viewport,
			&tempX,&tempY,&tempZ);
		dVector3 point2;
		point2[0]=tempX;
		point2[1]=tempY;
		point2[2]=tempZ;
		//获取x,realY在远侧裁剪平面处对应的物体坐标系坐标（反转到棋盘绘制时的坐标系）

		if(select_ray) dGeomDestroy(select_ray);
		select_ray = dCreateRay(space, 100);
		dGeomRaySet(select_ray,point1[0],point1[1],point1[2],point2[0],point2[1],point2[2]);
		//获得由近侧裁剪平面处鼠标点的地方发出的射线
		selected = -1; // 重新开始选择
		Invalidate(FALSE);
	}
	else if(nFlags & MK_CONTROL) // Ctrl + LButton : dragging to move a body
	{
		if(selected >= 0)
		{
			dBodySetLinearVel(obj[selected].body,0,0,0);
			dBodySetAngularVel(obj[selected].body,0,0,0);
			lastx = point.x;
			lasty = point.y;
			SetCapture();
			moving = TRUE;
		}
	}
	else
	{
		button |= 1;
		lastx = point.x;
		lasty = point.y;
		SetCapture();
	}
}

void CMyPhysicsView::OnMButtonDown(UINT nFlags, CPoint point)
{
	button |= 2;
	lastx = point.x;
	lasty = point.y;
	SetCapture();
}

void CMyPhysicsView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(nFlags & MK_SHIFT)
	{
		if(ground_box)
		{
			SetCapture();
			slope_adjust = TRUE;
		}
	}
	else
	{
		button |= 4;
		lastx = point.x;
		lasty = point.y;
		SetCapture();
	}
}

void CMyPhysicsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	button &= ~1;
	if (button==0) ReleaseCapture();
	moving = FALSE;
}

void CMyPhysicsView::OnMButtonUp(UINT nFlags, CPoint point)
{
	button &= ~2;
	if (button==0) ReleaseCapture();
}

void CMyPhysicsView::OnRButtonUp(UINT nFlags, CPoint point)
{
	button &= ~4;
	if (button==0) ReleaseCapture();
	slope_adjust = FALSE;
}

void CMyPhysicsView::OnMouseMove(UINT nFlags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if (button) dsMotion (button,x-lastx,y-lasty);
	if(slope_adjust)
	{
		if(slope_angle > 0 && y-lasty > 0 || slope_angle < M_PI && y-lasty < 0)
		{
			slope_angle -= (y-lasty)/1000.0;
		}
		else if(slope_angle < 0)
		{
			slope_angle = 0;
		}
		else if(slope_angle > M_PI)
		{
			slope_angle = M_PI;
		}
		dVector3 result;
		dGeomBoxGetLengths(ground_box,result);
		dMatrix3 R;
		dRFromAxisAndAngle (R,0,1,0,slope_angle);
		dGeomSetPosition (ground_box, -0.5*result[0]*cos(slope_angle), 0, 0.5*result[0]*sin(slope_angle));
		dGeomSetRotation (ground_box,R);
	}
	if(moving && selected >= 0)
	{
		GLint viewport[4];
		GLdouble mvMatrix[16],projMatrix[16];
		glGetIntegerv (GL_VIEWPORT,viewport);
		glGetDoublev (GL_MODELVIEW_MATRIX,mvMatrix);
		glGetDoublev (GL_PROJECTION_MATRIX,projMatrix);
		//获得观察流水线上各种矩阵
		int realY=viewport[3]-y;
		int realLastY=viewport[3]-lasty;
		//Windows窗口坐标默认顶点在左上角，这里转化为Opengl默认的左下角

		double tempX,tempY,tempZ;
		gluUnProject ((double) x,(double) realY,0.0,
			mvMatrix,projMatrix,viewport,
			&tempX,&tempY,&tempZ);
		dVector3 point1;
		point1[0]=tempX;
		point1[1]=tempY;
		point1[2]=tempZ;
		//获取x,realY在近侧裁剪平面处对应的物体坐标系坐标（反转到棋盘绘制时的坐标系）

		gluUnProject ((double) lastx,(double) realLastY,0.0,
			mvMatrix,projMatrix,viewport,
			&tempX,&tempY,&tempZ);
		//获取lastx,realLastY在近侧裁剪平面处对应的物体坐标系坐标（反转到棋盘绘制时的坐标系）
		dVector3 point2;
		point2[0]=tempX;
		point2[1]=tempY;
		point2[2]=tempZ;

		const dReal* pos = dBodyGetPosition(obj[selected].body);
		double re = 9.5*dDISTANCE(point1,pos); // 透视移动补偿
		tempX = pos[0]+(point1[0]-point2[0])*re;
		tempY = pos[1]+(point1[1]-point2[1])*re;
		tempZ = max(0,pos[2]+(point1[2]-point2[2])*re);
		dBodySetPosition(obj[selected].body,tempX,tempY,tempZ);
	}
	lastx = x;
	lasty = y;
}

void CMyPhysicsView::OnUpdateData() // used when the check boxes are clicked
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
}

void CMyPhysicsView::OnBnClickedStep()
{
	// TODO: 在此添加控件通知处理程序代码
	renderer_singlestep = TRUE;
	renderer_pause = TRUE;
	BNpause.SetWindowText(_T("继续"));
}

void CMyPhysicsView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	int res = MessageBox(_T("保存当前场景吗？"),0,MB_YESNOCANCEL);
	if(res == IDCANCEL)
	{
		return;
	}
	else if(res == IDYES)
	{
		OnFileSave();
	}
	// if res == IDYES or IDNO
	for(int i = 0; i< num; i++)
	{
		dBodyDestroy (obj[i].body);
		if (obj[i].geom) dGeomDestroy (obj[i].geom);
		memset (&obj[i],0,sizeof(obj[i]));
	}
	num = 0;
	if(ground_box)
	{
		dGeomDestroy(ground_box);
		ground_box = 0;
	}
	feedbackvector.reserve(0);
	std::vector<MyFeedback>().swap(feedbackvector); // vector 释放内存

	view_xyz[0] = 0.6317f;
	view_xyz[1] = -2.2817f;
	view_xyz[2] = 1.3000f;
	view_hpr[0] = 110.0000f;
	view_hpr[1] = -17.5000f;
	view_hpr[2] = 0.0000f;

	bshow_force = FALSE;
	use_textures = TRUE;
	use_shadows = TRUE;
	renderer_pause = FALSE;
	UpdateData(FALSE); // output data
	OnSpecifyScene();
}

void CMyPhysicsView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	BOOL pause_cache = renderer_pause;
	renderer_pause = TRUE;
	CFileDialog dlg(FALSE, _T(".scn"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("MyPhysics Files (*.scn)|*.scn|所有文件 (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		szFileName = dlg.GetFileName();//文件名
		write_world = TRUE;
	}
	renderer_pause = pause_cache;
}

BOOL CMyPhysicsView::PreTranslateMessage(MSG *pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			UpdateData(TRUE); // input data
			if(ForceScale > 5) ForceScale = 5;
			if(ForceScale <= 0) ForceScale = 1;
			nForceScale = 100-20*ForceScale;
			UpdateData(FALSE); // output data
			break;
		default:
			break;
		}
	}
	return FALSE;
}

void CMyPhysicsView::OnEnKillfocusScale()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	if(ForceScale > 5) ForceScale = 5;
	if(ForceScale <= 0) ForceScale = 1;
	nForceScale = 100-20*ForceScale;
	UpdateData(FALSE); // output data
}

void CMyPhysicsView::OnNMCustomdrawSliderScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // input data
	ForceScale = (100-nForceScale) / 20.0;
	UpdateData(FALSE); // output data
	*pResult = 0;
}

void CMyPhysicsView::OnSpecifyScene()
{
	// TODO: 在此添加命令处理程序代码
	CSceneDlg dlg(G, dWorldGetLinearDamping(world), dWorldGetAngularDamping(world), Mu, Bounce);
	if(	dlg.DoModal() == IDCANCEL) return;

	G = dlg.g;
	if(dlg.mu_infinity) Mu = dInfinity;
	else Mu = dlg.mu;
	Bounce = dlg.r;

	dWorldSetGravity (world,0,0,-G);
	dWorldSetLinearDamping(world, dlg.ldamping);
	dWorldSetAngularDamping(world, dlg.adamping);
}

void CMyPhysicsView::OnSelectCancel()
{
	// TODO: 在此添加命令处理程序代码
	selected = -1;
	Invalidate(FALSE);
}

void CMyPhysicsView::OnUpdateSelect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(selected >= 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMyPhysicsView::OnSelectNext()
{
	// TODO: 在此添加命令处理程序代码
	if(selected < num-1)
		selected++;
	else
		selected = -1;
	Invalidate(FALSE);
}

void CMyPhysicsView::OnBnClickedPlayPause()
{
	// TODO: 在此添加控件通知处理程序代码
	renderer_pause = !renderer_pause;
	if(renderer_pause)
	{
		BNpause.SetWindowText(_T("继续"));
	}
	else
	{
		BNpause.SetWindowText(_T("暂停"));
	}
}
