#ifndef dDOUBLE
#define dDOUBLE
#endif

#ifndef __DRAWSTUFF_H__
#define __DRAWSTUFF_H__

#include <gl/glu.h>
#include <ode/ode.h>

#ifndef DEFAULT_PATH_TO_TEXTURES
#ifdef WIN32
#define DEFAULT_PATH_TO_TEXTURES "..\\textures\\"
#else
#define DEFAULT_PATH_TO_TEXTURES "../textures/"
#endif
#endif

// some constants

#define NUM 50			// max number of objects
#define MAX_CONTACTS 8          // maximum number of contact points per body
#define WORLDSTEP 0.02     // the step time of each frame

// constants to convert degrees to radians and the reverse
#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD (M_PI/180.0)

#define Round(a) floor(a+0.5)

// light vector. LIGHTZ is implicitly 1
#define LIGHTX (1.0f)
#define LIGHTY (0.4f)

// ground and sky
#define SHADOW_INTENSITY (0.65f)
#define GROUND_R (0.5f) 	// ground color for when there's no texture
#define GROUND_G (0.5f)
#define GROUND_B (0.3f)

const float ground_scale = 1.0f/1.0f;	// ground texture scale (1/size)
const float ground_ofsx = 0.5;		// offset of ground texture
const float ground_ofsy = 0.5;
const float sky_scale = 1.0f/4.0f;	// sky texture scale (1/size)
const float sky_height = 1.0f;		// sky height above viewpoint

struct MyObject
{
	dBodyID body;      // the body
	dGeomID geom;      // geometry representing this body
	dVector3 force;    // the constant force add to the body
	dVector3 torque;   // the constant moment add to the body
	float color[4];
};

struct MyFeedback
{
	dJointFeedback fb;
	dBodyID body1;
	dBodyID body2;
};

/*
In the virtual world, the z axis is "up" and z=0 is the floor.

The user is able to click+drag in the main window to move the camera:
* left button - pan and tilt.
* right button - forward and sideways.
* left + right button (or middle button) - sideways and up.
*/


#if !defined(DS_API)
#define DS_API
#endif

#ifndef __VERSION_H
#define __VERSION_H

/* high byte is major version, low byte is minor version */
#define DS_VERSION 0x0002

#endif

/* texture numbers */
enum DS_TEXTURE_NUMBER
{
	DS_NONE = 0,       /* uses the current color instead of a texture */
	DS_WOOD,
	DS_CHECKERED,
	DS_GROUND,
	DS_SKY,
};

/* draw modes */

#define DS_POLYFILL  0
#define DS_WIREFRAME 1

/**
* @struct dsFunctions
* @brief Set of functions to be used as callbacks by the simulation loop.
* @ingroup drawstuff
*/
struct dsFunctions {
	int version;			/* put DS_VERSION here */
	/* version 1 data */
	void (*start)();		/* called before sim loop starts */
	void (*step) (int pause);	/* called before every frame */
	void (*command) (int cmd);	/* called if a command key is pressed */
	void (*stop)();		/* called after sim loop exits */
	/* version 2 data */
	const char *path_to_textures;	/* if nonzero, path to texture files */
};


//***************************************************************************
// PPM image object

typedef unsigned char byte;

class Image {
	int image_width,image_height;
	byte *image_data;
public:
	Image (char *filename);
	// load from PPM file
	~Image();
	int width() { return image_width; }
	int height() { return image_height; }
	byte *data() { return image_data; }
};


//***************************************************************************
// Texture object.

class Texture {
	Image *image;
	GLuint name;
public:
	Texture (char *filename);
	~Texture();
	void bind (int modulate);
};

//***************************************************************************
// OpenGL utility stuff

void setCamera (float x, float y, float z, float h, float p, float r);

// sets the material color, not the light color

void setColor (float r, float g, float b, float alpha);
void setTransform (const dReal pos[3], const dReal R[12]);
//void setTransformD (const double pos[3], const double R[12]);

//***************************************************************************

void setShadowTransform();

void drawConvex (dReal *_planes,unsigned int _planecount,
				 dReal *_points,
				 unsigned int _pointcount,
				 unsigned int *_polygons);

void drawBox (const dReal sides[3]);

void drawPatch (float p1[3], float p2[3], float p3[3], int level);

void drawSphere();

void drawSphereShadow (float px, float py, float pz, float radius);

void drawTriangle (const dReal *v0, const dReal *v1, const dReal *v2, int solid);

void drawCapsule (float l, float r);

void drawCylinder (float l, float r, float zoffset);

void normalizeVector3 (float v[3]);

#endif
