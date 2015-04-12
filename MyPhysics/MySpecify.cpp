/*
创建、删除、设置物体时的响应函数
*/
#include "stdafx.h"

#include "SpecifyDlg.h"
#include "SpecifySlope.h"
#include "DynamicDlg.h"
#include "MyPhysicsView.h"

void CMyPhysicsView::OnSphere()
{
	CSpecifySphere specify_sphere;
	if(specify_sphere.DoModal() == IDCANCEL) return;

	size_t i;

	if (num < NUM)
	{
		i = num;
		num++;
	}
	else
	{
		i = nextobj;
		nextobj++;
		if (nextobj >= num) nextobj = 0;

		// destroy the body and geoms for slot i
		dBodyDestroy (obj[i].body);
		if (obj[i].geom) dGeomDestroy (obj[i].geom);
		memset (&obj[i],0,sizeof(obj[i]));
	}

	obj[i].body = dBodyCreate (world);
	dBodySetPosition (obj[i].body,
		specify_sphere.posX, specify_sphere.posY, specify_sphere.posZ);
	dBodySetRotation (obj[i].body, specify_sphere.R);
	dBodySetData (obj[i].body,(void*) i);

	obj[i].geom = dCreateSphere (space,specify_sphere.sizeR);

	if (obj[i].geom) dGeomSetBody (obj[i].geom,obj[i].body);

	dBodySetMass (obj[i].body, &specify_sphere.Mass);
	obj[i].color[0] = specify_sphere.ncolorR / 100.0;
	obj[i].color[1] = specify_sphere.ncolorG / 100.0;
	obj[i].color[2] = specify_sphere.ncolorB / 100.0;
	obj[i].color[3] = specify_sphere.ncolorAlpha / 100.0;
}

void CMyPhysicsView::OnBox()
{
	CSpecifyBox specify_box;
	if(specify_box.DoModal() == IDCANCEL) return;

	size_t i;

	if (num < NUM)
	{
		i = num;
		num++;
	}
	else
	{
		i = nextobj;
		nextobj++;
		if (nextobj >= num) nextobj = 0;

		// destroy the body and geoms for slot i
		dBodyDestroy (obj[i].body);
		if (obj[i].geom) dGeomDestroy (obj[i].geom);
		memset (&obj[i],0,sizeof(obj[i]));
	}

	obj[i].body = dBodyCreate (world);
	dBodySetPosition (obj[i].body,
		specify_box.posX, specify_box.posY, specify_box.posZ);
	dBodySetRotation (obj[i].body, specify_box.R);
	dBodySetData (obj[i].body,(void*) i);

	obj[i].geom = dCreateBox (space,
		specify_box.sizeX, specify_box.sizeY, specify_box.sizeZ);

	if (obj[i].geom) dGeomSetBody (obj[i].geom,obj[i].body);
	dBodySetMass (obj[i].body, &specify_box.Mass);
	obj[i].color[0] = specify_box.ncolorR / 100.0;
	obj[i].color[1] = specify_box.ncolorG / 100.0;
	obj[i].color[2] = specify_box.ncolorB / 100.0;
	obj[i].color[3] = specify_box.ncolorAlpha / 100.0;
}

void CMyPhysicsView::OnSlope()
{
	if(ground_box)
	{
		dGeomDestroy(ground_box);
		ground_box = 0;
		slope_angle = 0.2618;
	}
	else
	{
		CspecifySlope slope;
		if(slope.DoModal() == IDCANCEL) return;

		conveyor_vel = slope.vel;
		conveyor_dirup = (slope.up == 0);
		slope_angle = slope.radian;
		ground_box = dCreateBox (space,slope.l,slope.l,slope.h);
		dMatrix3 R;
		dRFromAxisAndAngle (R,0,1,0,slope_angle);
		dGeomSetPosition (ground_box, -0.5*slope.l*cos(slope_angle), 0, 0.5*slope.l*sin(slope_angle));
		dGeomSetRotation (ground_box,R);
	}
}

void CMyPhysicsView::OnDelete()
{
	if(selected >= 0)
	{
		if(MessageBox(_T("要删除该物体吗？"),NULL,MB_YESNO)==IDYES)
		{
			dBodyDestroy(obj[selected].body);
			if (obj[selected].geom) dGeomDestroy (obj[selected].geom);
			if (selected != num-1) memcpy(&obj[selected],&obj[num-1],sizeof(obj[selected]));
			num--;
			selected = -1;
		}
	}
}

void CMyPhysicsView::OnSetStatic()
{
	if(selected == -1) return;
	switch(dGeomGetClass(obj[selected].geom))
	{
	case dSphereClass:
		{
			CSpecifySphere specify_sphere(&obj[selected]);
			if(specify_sphere.DoModal() == IDCANCEL) return;

			dBodySetPosition (obj[selected].body,
				specify_sphere.posX, specify_sphere.posY, specify_sphere.posZ);
			dBodySetRotation (obj[selected].body, specify_sphere.R);

			dGeomSphereSetRadius(obj[selected].geom, specify_sphere.sizeR);

			dBodySetMass (obj[selected].body, &specify_sphere.Mass);
			obj[selected].color[0] = specify_sphere.ncolorR / 100.0;
			obj[selected].color[1] = specify_sphere.ncolorG / 100.0;
			obj[selected].color[2] = specify_sphere.ncolorB / 100.0;
			obj[selected].color[3] = specify_sphere.ncolorAlpha / 100.0;
			break;
		}
	case dBoxClass:
		{
			CSpecifyBox specify_box(&obj[selected]);
			if(specify_box.DoModal() == IDCANCEL) return;

			dBodySetPosition (obj[selected].body,
				specify_box.posX, specify_box.posY, specify_box.posZ);
			dBodySetRotation (obj[selected].body, specify_box.R);

			dGeomBoxSetLengths(obj[selected].geom,
				specify_box.sizeX, specify_box.sizeY, specify_box.sizeZ);

			dBodySetMass (obj[selected].body, &specify_box.Mass);
			obj[selected].color[0] = specify_box.ncolorR / 100.0;
			obj[selected].color[1] = specify_box.ncolorG / 100.0;
			obj[selected].color[2] = specify_box.ncolorB / 100.0;
			obj[selected].color[3] = specify_box.ncolorAlpha / 100.0;
			break;
		}
	case dCapsuleClass:
		MessageBox(_T("Capsule!"));
		break;
	case dCylinderClass:
		MessageBox(_T("Cylinder!"));
		break;
	default:
		MessageBox(_T("I don't know you!"));
	}
}

void CMyPhysicsView::OnSetDynamic()
{
	if(selected == -1) return;
	CDynamicDlg dlg(&obj[selected]);
	if(dlg.DoModal() == IDCANCEL) return;
	dBodySetLinearVel(obj[selected].body,dlg.vlx,dlg.vly,dlg.vlz);
	dBodySetAngularVel(obj[selected].body,dlg.vax,dlg.vay,dlg.vaz);
	// add impulse
	dVector3 force;
	dWorldImpulseToForce(world,WORLDSTEP,dlg.ilx,dlg.ily,dlg.ilz,force);
	dBodyAddForce(obj[selected].body,force[0],force[1],force[2]);

	dWorldImpulseToForce(world,WORLDSTEP,dlg.iax,dlg.iay,dlg.iaz,force);
	dBodyAddTorque(obj[selected].body,force[0],force[1],force[2]);
	// set force
	obj[selected].force[0] = dlg.flx;
	obj[selected].force[1] = dlg.fly;
	obj[selected].force[2] = dlg.flz;
	obj[selected].torque[0] = dlg.fax;
	obj[selected].torque[1] = dlg.fay;
	obj[selected].torque[2] = dlg.faz;
}
