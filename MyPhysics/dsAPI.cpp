#include "stdafx.h"

#include "MyPhysicsView.h"

void CMyPhysicsView::initMotionModel()
{
	view_xyz[0] = 2;
	view_xyz[1] = 0;
	view_xyz[2] = 1;
	view_hpr[0] = 180;
	view_hpr[1] = 0;
	view_hpr[2] = 0;
}

void CMyPhysicsView::wrapCameraAngles()
{
	for (int i=0; i<3; i++)
	{
		while (view_hpr[i] > 180) view_hpr[i] -= 360;
		while (view_hpr[i] < -180) view_hpr[i] += 360;
	}
}

// call this to update the current camera position. the bits in `mode' say
// if the left (1), middle (2) or right (4) mouse button is pressed, and
// (deltax,deltay) is the amount by which the mouse pointer has moved.

void CMyPhysicsView::dsMotion (int mode, int deltax, int deltay)
{
	float side = 0.01f * float(deltax);
	float fwd = (mode==4) ? (0.01f * float(deltay)) : 0.0f;
	float s = (float) sin (view_hpr[0]*DEG_TO_RAD);
	float c = (float) cos (view_hpr[0]*DEG_TO_RAD);

	if (mode==1) {
		view_hpr[0] += float (deltax) * 0.5f;
		view_hpr[1] += float (deltay) * 0.5f;
	}
	else {
		view_xyz[0] += -s*side + c*fwd;
		view_xyz[1] += c*side + s*fwd;
		if (mode==2 || mode==5) view_xyz[2] += 0.01f * float(deltay);
	}
	wrapCameraAngles();
}

//***************************************************************************
// drawing loop stuff

void CMyPhysicsView::dsStartGraphics (int width, int height, dsFunctions *fn)
{

	const char *prefix = DEFAULT_PATH_TO_TEXTURES;
	if (fn->version >= 2 && fn->path_to_textures) prefix = fn->path_to_textures;
	size_t size_s = strlen(prefix) + 20;
	char *s = (char*) alloca (size_s);

	//strcpy (s,prefix);
	strcpy_s(s,size_s,prefix);
	//strcat (s,"/sky.ppm");
	strcat_s(s,size_s,"/sky.ppm");
	texture[DS_SKY] = sky_texture = new Texture (s);

	//strcpy (s,prefix);
	strcpy_s(s,size_s,prefix);
	//strcat (s,"/ground.ppm");
	strcat_s(s,size_s,"/ground.ppm");
	texture[DS_GROUND] = ground_texture = new Texture (s);

	//strcpy (s,prefix);
	strcpy_s(s,size_s,prefix);
	//strcat (s,"/wood.ppm");
	strcat_s(s,size_s,"/wood.ppm");
	texture[DS_WOOD] = wood_texture = new Texture (s);

	//strcpy (s,prefix);
	strcpy_s(s,size_s,prefix);
	//strcat (s,"/checkered.ppm");
	strcat_s(s,size_s,"/checkered.ppm");
	texture[DS_CHECKERED] = checkered_texture = new Texture (s);
}


void CMyPhysicsView::dsStopGraphics()
{
	delete sky_texture;
	delete ground_texture;
	delete wood_texture;
	sky_texture = 0;
	ground_texture = 0;
	wood_texture = 0;
}


void CMyPhysicsView::drawSky (float view_xyz[3])
{
	glDisable (GL_LIGHTING);
	if (use_textures) {
		glEnable (GL_TEXTURE_2D);
		sky_texture->bind (0);
	}
	else {
		glDisable (GL_TEXTURE_2D);
		glColor3f (0,0.5,1.0);
	}

	// make sure sky depth is as far back as possible
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glDepthRange (1,1);

	const float ssize = 1000.0f;
	static float offset = 0.0f;

	float x = ssize*sky_scale;
	float z = view_xyz[2] + sky_height;

	glBegin (GL_QUADS);
	glNormal3f (0,0,-1);
	glTexCoord2f (-x+offset,-x+offset);
	glVertex3f (-ssize+view_xyz[0],-ssize+view_xyz[1],z);
	glTexCoord2f (-x+offset,x+offset);
	glVertex3f (-ssize+view_xyz[0],ssize+view_xyz[1],z);
	glTexCoord2f (x+offset,x+offset);
	glVertex3f (ssize+view_xyz[0],ssize+view_xyz[1],z);
	glTexCoord2f (x+offset,-x+offset);
	glVertex3f (ssize+view_xyz[0],-ssize+view_xyz[1],z);
	glEnd();

	offset = offset + 0.003f;
	if (offset > 1) offset -= 1;

	glDepthFunc (GL_LESS);
	glDepthRange (0,1);
}


void CMyPhysicsView::drawGround()
{
	glDisable (GL_LIGHTING);
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);
	// glDepthRange (1,1);

	if (use_textures) {
		glEnable (GL_TEXTURE_2D);
		ground_texture->bind (0);
	}
	else {
		glDisable (GL_TEXTURE_2D);
		glColor3f (GROUND_R,GROUND_G,GROUND_B);
	}

	const float gsize = 100.0f;
	const float offset = 0; // -0.001f; ... polygon offsetting doesn't work well

	glBegin (GL_QUADS);
	glNormal3f (0,0,1);
	glTexCoord2f (-gsize*ground_scale + ground_ofsx,
		-gsize*ground_scale + ground_ofsy);
	glVertex3f (-gsize,-gsize,offset);
	glTexCoord2f (gsize*ground_scale + ground_ofsx,
		-gsize*ground_scale + ground_ofsy);
	glVertex3f (gsize,-gsize,offset);
	glTexCoord2f (gsize*ground_scale + ground_ofsx,
		gsize*ground_scale + ground_ofsy);
	glVertex3f (gsize,gsize,offset);
	glTexCoord2f (-gsize*ground_scale + ground_ofsx,
		gsize*ground_scale + ground_ofsy);
	glVertex3f (-gsize,gsize,offset);
	glEnd();

	glDisable (GL_FOG);
}


void CMyPhysicsView::drawPyramidGrid()
{
	// setup stuff
	glEnable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	// draw the pyramid grid
	for (int i=-1; i<=1; i++) {
		for (int j=-1; j<=1; j++) {
			glPushMatrix();
			glTranslatef ((float)i,(float)j,(float)0);
			if (i==1 && j==0) setColor (1,0,0,1);
			else if (i==0 && j==1) setColor (0,0,1,1);
			else setColor (1,1,0,1);
			const float k = 0.03f;
			glBegin (GL_TRIANGLE_FAN);
			glNormal3f (0,-1,1);
			glVertex3f (0,0,k);
			glVertex3f (-k,-k,0);
			glVertex3f ( k,-k,0);
			glNormal3f (1,0,1);
			glVertex3f ( k, k,0);
			glNormal3f (0,1,1);
			glVertex3f (-k, k,0);
			glNormal3f (-1,0,1);
			glVertex3f (-k,-k,0);
			glEnd();
			glPopMatrix();
		}
	}
}


void CMyPhysicsView::dsDrawFrame (int width, int height, dsFunctions *fn, int pause)
{
	if (current_state < 1) ASSERT ("internal error");
	current_state = 2;

	// setup stuff
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);

	// setup viewport
	glViewport (0,0,width,height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	const float vnear = 0.1f;
	const float vfar = 100.0f;
	const float k = 0.8f;     // view scale, 1 = +/- 45 degrees
	if (width >= height) {
		float k2 = float(height)/float(width);
		glFrustum (-vnear*k,vnear*k,-vnear*k*k2,vnear*k*k2,vnear,vfar);
	}
	else {
		float k2 = float(width)/float(height);
		glFrustum (-vnear*k*k2,vnear*k*k2,-vnear*k,vnear*k,vnear,vfar);
	}

	// setup lights. it makes a difference whether this is done in the
	// GL_PROJECTION matrix mode (lights are scene relative) or the
	// GL_MODELVIEW matrix mode (lights are camera relative, bad!).
	static GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
	glColor3f (1.0, 1.0, 1.0);

	// clear the window
	glClearColor (0.5,0.5,0.5,0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// snapshot camera position (in MS Windows it is changed by the GUI thread)
	float view2_xyz[3];
	float view2_hpr[3];
	memcpy (view2_xyz,view_xyz,sizeof(float)*3);
	memcpy (view2_hpr,view_hpr,sizeof(float)*3);

	// go to GL_MODELVIEW matrix mode and set the camera
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	setCamera (view2_xyz[0],view2_xyz[1],view2_xyz[2],
		view2_hpr[0],view2_hpr[1],view2_hpr[2]);

	// set the light position (for some reason we have to do this in model view.
	static GLfloat light_position[] = { LIGHTX, LIGHTY, 1.0, 0.0 };
	glLightfv (GL_LIGHT0, GL_POSITION, light_position);

	// draw the background (ground, sky etc)
	drawSky (view2_xyz);
	drawGround();

	// draw the little markers on the ground
	drawPyramidGrid();

	// leave openGL in a known state - flat shaded white, no textures
	glEnable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);
	glColor3f (1,1,1);
	setColor (1,1,1,1);

	// draw the rest of the objects. set drawing state first.
	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;
	tnum = 0;
	if (fn->step) fn->step (pause);
}

////////////////////////////////////

void CMyPhysicsView::dsSetViewpoint (float xyz[3], float hpr[3])
{
	if (current_state < 1) ASSERT ("dsSetViewpoint() called before simulation started");
	if (xyz) {
		view_xyz[0] = xyz[0];
		view_xyz[1] = xyz[1];
		view_xyz[2] = xyz[2];
	}
	if (hpr) {
		view_hpr[0] = hpr[0];
		view_hpr[1] = hpr[1];
		view_hpr[2] = hpr[2];
		wrapCameraAngles();
	}
}

void CMyPhysicsView::dsGetViewpoint (float xyz[3], float hpr[3])
{
	if (current_state < 1) ASSERT ("dsGetViewpoint() called before simulation started");
	if (xyz) {
		xyz[0] = view_xyz[0];
		xyz[1] = view_xyz[1];
		xyz[2] = view_xyz[2];
	}
	if (hpr) {
		hpr[0] = view_hpr[0];
		hpr[1] = view_hpr[1];
		hpr[2] = view_hpr[2];
	}
}

void CMyPhysicsView::dsSetTexture (int texture_number)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	tnum = texture_number;
}

void CMyPhysicsView::dsSetColor (float red, float green, float blue)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = 1;
}

void CMyPhysicsView::dsSetColorAlpha (float red, float green, float blue, float alpha)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
}

void CMyPhysicsView::dsDrawBox (const dReal pos[3], const dReal R[12],
								const dReal sides[3])
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glShadeModel (GL_FLAT);
	setTransform (pos,R);
	drawBox (sides);
	glPopMatrix();

	if (use_shadows) {
		setShadowDrawingMode();
		setShadowTransform();
		setTransform (pos,R);
		drawBox (sides);
		glPopMatrix();
		glPopMatrix();
		glDepthRange (0,1);
	}
}

void CMyPhysicsView::dsDrawConvex (const dReal pos[3], const dReal R[12],
								   dReal *_planes,unsigned int _planecount,
								   dReal *_points,
								   unsigned int _pointcount,
								   unsigned int *_polygons)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glShadeModel (GL_FLAT);
	setTransform (pos,R);
	drawConvex(_planes,_planecount,_points,_pointcount,_polygons);
	glPopMatrix();
	if (use_shadows) {
		setShadowDrawingMode();
		setShadowTransform();
		setTransform (pos,R);
		drawConvex(_planes,_planecount,_points,_pointcount,_polygons);
		glPopMatrix();
		glPopMatrix();
		glDepthRange (0,1);
	}
}

void CMyPhysicsView::dsDrawSphere (const dReal pos[3], const dReal R[12],
								   dReal radius)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glEnable (GL_NORMALIZE);
	glShadeModel (GL_SMOOTH);
	setTransform (pos,R);
	glScaled (radius,radius,radius);
	drawSphere();
	glPopMatrix();
	glDisable (GL_NORMALIZE);

	// draw shadows
	if (use_shadows) {
		glDisable (GL_LIGHTING);
		if (use_textures) {
			ground_texture->bind (1);
			glEnable (GL_TEXTURE_2D);
			glDisable (GL_TEXTURE_GEN_S);
			glDisable (GL_TEXTURE_GEN_T);
			glColor3f (SHADOW_INTENSITY,SHADOW_INTENSITY,SHADOW_INTENSITY);
		}
		else {
			glDisable (GL_TEXTURE_2D);
			glColor3f (GROUND_R*SHADOW_INTENSITY,GROUND_G*SHADOW_INTENSITY,
				GROUND_B*SHADOW_INTENSITY);
		}
		glShadeModel (GL_FLAT);
		glDepthRange (0,0.9999);
		drawSphereShadow (pos[0],pos[1],pos[2],radius);
		glDepthRange (0,1);
	}
}

void CMyPhysicsView::dsDrawTriangle (const dReal pos[3], const dReal R[12],
									 const dReal *v0, const dReal *v1,
									 const dReal *v2, int solid)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glShadeModel (GL_FLAT);
	setTransform (pos,R);
	drawTriangle (v0, v1, v2, solid);
	glPopMatrix();
}

void CMyPhysicsView::dsDrawCylinder (const dReal pos[3], const dReal R[12],
									 float length, float radius)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glShadeModel (GL_SMOOTH);
	setTransform (pos,R);
	drawCylinder (length,radius,0);
	glPopMatrix();

	if (use_shadows) {
		setShadowDrawingMode();
		setShadowTransform();
		setTransform (pos,R);
		drawCylinder (length,radius,0);
		glPopMatrix();
		glPopMatrix();
		glDepthRange (0,1);
	}
}

void CMyPhysicsView::dsDrawCapsule (const dReal pos[3], const dReal R[12],
									float length, float radius)
{
	if (current_state != 2) ASSERT ("drawing function called outside simulation loop");
	setupDrawingMode();
	glShadeModel (GL_SMOOTH);
	setTransform (pos,R);
	drawCapsule (length,radius);
	glPopMatrix();

	if (use_shadows) {
		setShadowDrawingMode();
		setShadowTransform();
		setTransform (pos,R);
		drawCapsule (length,radius);
		glPopMatrix();
		glPopMatrix();
		glDepthRange (0,1);
	}
}


void CMyPhysicsView::dsDrawLine (const dReal pos1[3], const dReal pos2[3])
{
	setupDrawingMode();
	glColor3f (color[0],color[1],color[2]);
	glDisable (GL_LIGHTING);
	glLineWidth (3);
	glShadeModel (GL_FLAT);
	glBegin (GL_LINES);
	glVertex3f (pos1[0],pos1[1],pos1[2]);
	glVertex3f (pos2[0],pos2[1],pos2[2]);
	glEnd();
}

void CMyPhysicsView::dsDrawArrow (const dReal pos1[3], const dReal pos2[3])
{
	if(pos1[0] == pos2[0] && pos1[1] == pos2[1] && pos1[2] == pos2[2]) return;
	dsDrawLine (pos1, pos2);

	dMatrix3 R;
	dRSetIdentity(R);
	glEnable (GL_NORMALIZE);
	glShadeModel (GL_SMOOTH);
	setTransform (pos2,R);
	glScaled (0.02,0.02,0.02);
	drawSphere();
	glPopMatrix();
	glDisable (GL_NORMALIZE);
}

//void CMyPhysicsView::dsDrawArrow (const dReal pos1[3], const dReal pos2[3])
//{
//	if(pos1[0] == pos2[0] && pos1[1] == pos2[1] && pos1[2] == pos2[2]) return;
//	dsDrawLine (pos1, pos2);
//	static dReal offset = 0;
//	offset += 0.02;
//	if(offset >= 2*M_PI) offset = 0;
//
//	dMatrix3 R;
//	dRSetIdentity(R);
//	dVector3 dir={pos2[0]-pos1[0],
//				  pos2[1]-pos1[1],
//				  pos2[2]-pos1[2]};
//	dNormalize3(dir);
//	R[0*4+2] = dir[0];
//	R[1*4+2] = dir[1];
//	R[2*4+2] = dir[2];
//
//	dVector3 pos3;
//	dVector3 temp;
//
//	temp[0] = 0.05*cos(offset);
//	temp[1] = 0.05*sin(offset);
//	temp[2] = dDISTANCE(pos1,pos2)-0.1;
//	dMULTIPLYOP0_133(pos3,=,temp,R);
//	//dOPE(pos3,+=,pos2);
//	dsDrawLine (pos3, pos2);
//
//	temp[0] = -0.05*cos(offset);
//	temp[1] = -0.05*sin(offset);
//	temp[2] = dDISTANCE(pos1,pos2)-0.1;
//	dMULTIPLYOP0_133(pos3,=,temp,R);
//	//dOPE(pos3,+=,pos2);
//	dsDrawLine (pos3, pos2);
//
//}



void CMyPhysicsView::setupDrawingMode()
{
	glEnable (GL_LIGHTING);
	if (tnum)
	{
		if (use_textures)
		{
			glEnable (GL_TEXTURE_2D);
			texture[tnum]->bind (1);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glTexGeni (GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni (GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			static GLfloat s_params[4] = {1.0f,1.0f,0.0f,1};
			static GLfloat t_params[4] = {0.817f,-0.817f,0.817f,1};
			glTexGenfv (GL_S,GL_OBJECT_PLANE,s_params);
			glTexGenfv (GL_T,GL_OBJECT_PLANE,t_params);
		}
		else
		{
			glDisable (GL_TEXTURE_2D);
		}
	}
	else
	{
		glDisable (GL_TEXTURE_2D);
	}
	setColor (color[0],color[1],color[2],color[3]);

	if (color[3] < 1)
	{
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable (GL_BLEND);
	}
}


void CMyPhysicsView::setShadowDrawingMode()
{
	glDisable (GL_LIGHTING);
	if (use_textures)
	{
		glEnable (GL_TEXTURE_2D);
		ground_texture->bind (1);
		glColor3f (SHADOW_INTENSITY,SHADOW_INTENSITY,SHADOW_INTENSITY);
		glEnable (GL_TEXTURE_2D);
		glEnable (GL_TEXTURE_GEN_S);
		glEnable (GL_TEXTURE_GEN_T);
		glTexGeni (GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
		glTexGeni (GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
		static GLfloat s_params[4] = {ground_scale,0,0,ground_ofsx};
		static GLfloat t_params[4] = {0,ground_scale,0,ground_ofsy};
		glTexGenfv (GL_S,GL_EYE_PLANE,s_params);
		glTexGenfv (GL_T,GL_EYE_PLANE,t_params);
	}
	else
	{
		glDisable (GL_TEXTURE_2D);
		glColor3f (GROUND_R*SHADOW_INTENSITY,GROUND_G*SHADOW_INTENSITY,
			GROUND_B*SHADOW_INTENSITY);
	}
	glDepthRange (0,0.9999);
}
