#include "stdafx.h"

#include "MyPhysicsView.h"

void CMyPhysicsView::nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	// exit without doing anything if the two bodies are connected by a joint
	if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

	dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
	for (int i=0; i<MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactBounce;
		contact[i].surface.mu = Mu;
		contact[i].surface.bounce = Bounce;
		contact[i].surface.bounce_vel = 0.02;
		if(conveyor_vel > 0 && (o1 == ground_box || o2 == ground_box))
		{
			contact[i].surface.mode = dContactFDir1 | dContactMotion1;
			if(conveyor_dirup)
				contact[i].fdir1[0] = -cos(slope_angle);
			else
				contact[i].fdir1[0] = cos(slope_angle);
			contact[i].fdir1[1] = 0.0;
			contact[i].fdir1[2] = sin(slope_angle);
			contact[i].surface.motion1 = conveyor_vel;
		}
	}
	if (int numc = dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact)))
	{
		if(o1 == select_ray)
		{
			if(b2 == 0) return;
			for(int i=0; i<num; i++)
			{
				if(obj[i].geom == o2)
				{
					if(selected < 0)
					{
						selected = i;
					}
					else
					{
						const dReal* select = dGeomGetPosition(obj[selected].geom);
						const dReal* current = dGeomGetPosition(obj[i].geom);
						const dReal* raystart = dGeomGetPosition(select_ray);
						if(dDISTANCE(raystart,current) < dDISTANCE(raystart,select))
						{
							selected = i;
						}
					}
				}
			}
			select_finish = TRUE;
			return;
		}
		else if(o2 == select_ray)
		{
			if(b1 == 0) return;
			for(int i=0; i<num; i++)
			{
				if(obj[i].geom == o1)
				{
					if(selected < 0)
					{
						selected = i;
					}
					else
					{
						const dReal* select = dGeomGetPosition(obj[selected].geom);
						const dReal* current = dGeomGetPosition(obj[i].geom);
						const dReal* raystart = dGeomGetPosition(select_ray);
						if(dDISTANCE(raystart,current) < dDISTANCE(raystart,select))
						{
							selected = i;
						}
					}
				}
			}
			select_finish = TRUE;
			return;
		}
		for (int i=0; i<numc; i++)
		{
			dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
			dJointAttach (c,b1,b2);

			if(bshow_force)
			{
				if(feedbackvector.size() <= fbnum)
				{
					feedbackvector.resize(feedbackvector.size()*2+1);
				}
				feedbackvector[fbnum].body1 = b1;
				feedbackvector[fbnum].body2 = b2;
				dJointSetFeedback (c,&feedbackvector[fbnum++].fb);
			}
		}
	}
}

void CMyPhysicsView::drawGeom (dGeomID g, const dReal *pos, const dReal *R)
{
	if (!g) return;
	if (!pos) pos = dGeomGetPosition (g);
	if (!R) R = dGeomGetRotation (g);

	int type = dGeomGetClass (g);
	if (type == dBoxClass)
	{
		dVector3 sides;
		dGeomBoxGetLengths (g,sides);
		dsDrawBox (pos,R,sides);
	}
	else if (type == dSphereClass)
	{
		dsDrawSphere (pos,R,dGeomSphereGetRadius (g));
	}
	else if (type == dCapsuleClass)
	{
		dReal radius,length;
		dGeomCapsuleGetParams (g,&radius,&length);
		dsDrawCapsule (pos,R,length,radius);
	}
	else if (type == dCylinderClass)
	{
		dReal radius,length;
		dGeomCylinderGetParams (g,&radius,&length);
		dsDrawCylinder (pos,R,length,radius);
	}
}

void CMyPhysicsView::start()
{
	dAllocateODEDataForThread(dAllocateMaskAll);

	float xyz[3] = {0.6317f,-2.2817f,1.3000f};
	float hpr[3] = {110.0000f,-17.5000f,0.0000f};
	dsSetViewpoint (xyz,hpr);
}

void CMyPhysicsView::stop()
{
}

void CMyPhysicsView::simLoop (int pause)
{
	dSpaceCollide (space,NULL,&nearCallback);
	if (!pause) dWorldQuickStep (world,WORLDSTEP);

	// for saving file
	if (write_world)
	{
		USES_CONVERSION;
		const char* fname = T2A(szFileName);

		//FILE *f = fopen (fname,"wt");
		FILE *f;
		fopen_s(&f,fname,"wt");
		if (f)
		{
			dWorldExportDIF (world,f,"X");
			fclose (f);
		}
		write_world = FALSE;
	}
	// drawing forces between objects
	if (bshow_force)
	{
		bool draw = false; // 只有draw的时候画力，不然一直累加算合力
		const dReal* pos_start;
		dVector3 pos_end1;
		dVector3 pos_end2;
		dOPEC(pos_end1, =, 0); // pos_end1 = {0,0,0}
		dOPEC(pos_end2, =, 0); // pos_end2 = {0,0,0}

		// for every force
		for(size_t i = 0; i < fbnum; i++)
		{
			if(i == fbnum-1 // 最后一个力
				|| feedbackvector[i].body1 != feedbackvector[i+1].body1
				|| feedbackvector[i].body2 != feedbackvector[i+1].body2) // 或者两个物体之一变化了
			{
				draw = true;
			}
			if(feedbackvector[i].body1)
			{
				dOPE(pos_end1, +=, feedbackvector[i].fb.f1); // pos_end1+=fb.f1 --> sum the force
				if(draw)
				{
					dOPEC(pos_end1,*=,ForceScale); // pos_end1 *= ForceScale
					pos_start = dBodyGetPosition(feedbackvector[i].body1); // get the mass center as the start position
					dOP(pos_end1, +, pos_start, pos_end1); // calculate the end position

					dRandSetSeed((unsigned long long)feedbackvector[i].body1);
					dsSetColor(dRandReal(),dRandReal(),dRandReal());
					dRandSetSeed((UINT)time(NULL));

					dsDrawArrow(pos_start,pos_end1);
					dOPEC(pos_end1, =, 0); // pos_end1 = {0,0,0}
				}
			}
			if(feedbackvector[i].body2) // similar as above
			{
				dOPE(pos_end2, +=, feedbackvector[i].fb.f2);
				if(draw)
				{
					dOPEC(pos_end2,*=,ForceScale);
					pos_start = dBodyGetPosition(feedbackvector[i].body2);
					dOP(pos_end2, +, pos_start, pos_end2);

					dRandSetSeed((unsigned long long)feedbackvector[i].body2);
					dsSetColor(dRandReal(),dRandReal(),dRandReal());
					dRandSetSeed((UINT)time(NULL));

					dsDrawArrow(pos_start,pos_end2);
					dsDrawArrow(pos_start,pos_end2);
					dOPEC(pos_end2, =, 0);
				}
			}
			draw = false;
		}
	}
	fbnum = 0;

	// remove all contact joints
	dJointGroupEmpty (contactgroup);

	dsSetColor (1,1,0);
	dsSetTexture (DS_WOOD);

	// for every body
	for (int i=0; i<num; i++)
	{
		dBodyAddForce(obj[i].body,obj[i].force[0],obj[i].force[1],obj[i].force[2]);
		dBodyAddTorque(obj[i].body,obj[i].torque[0],obj[i].torque[1],obj[i].torque[2]);
		if(bshow_force)
		{
			// Draw Gravity
			dRandSetSeed((unsigned long long)obj[i].body);
			dsSetColor(dRandReal(),dRandReal(),dRandReal());
			dRandSetSeed((UINT)time(NULL));
			dMass m;
			const dReal* pos_start = dBodyGetPosition(obj[i].body);
			dVector3 pos_end;
			dBodyGetMass(obj[i].body, &m);
			pos_end[0] = pos_start[0];
			pos_end[1] = pos_start[1];
			pos_end[2] = pos_start[2] - m.mass * G * ForceScale;
			dsDrawArrow(pos_start,pos_end);
			// Draw Constant Force (if there is)
			if(obj[i].force[0] || obj[i].force[1] || obj[i].force[2])
			{
				pos_end[0] = pos_start[0] + obj[i].force[0] * ForceScale;
				pos_end[1] = pos_start[1] + obj[i].force[1] * ForceScale;
				pos_end[2] = pos_start[2] + obj[i].force[2] * ForceScale;
			}
			dsDrawArrow(pos_start,pos_end);
		}
		if (i==selected)
		{
			static int ctrl = 0;
			ctrl++;
			if(ctrl > 100) ctrl = 0;
			if(ctrl < 50)
			{
				dsSetColorAlpha(1-obj[i].color[0],1-obj[i].color[1],1-obj[i].color[2],obj[i].color[3]);
			}
			else
			{
				dsSetColorAlpha(obj[i].color[0],obj[i].color[1],obj[i].color[2],obj[i].color[3]);
			}
		}
		else
		{
			dsSetColorAlpha(obj[i].color[0],obj[i].color[1],obj[i].color[2],obj[i].color[3]);
		}
		drawGeom (obj[i].geom,0,0);
	}
	if(ground_box)
	{
		static float r = dRandReal();
		static float g = dRandReal();
		static float b = dRandReal();
		dsSetColor(r,g,b);

		dVector3 ss;
		dGeomBoxGetLengths (ground_box,ss);
		dsDrawBox (dGeomGetPosition(ground_box),dGeomGetRotation(ground_box),ss);
	}
	if(select_ray && select_finish)
	{
		dGeomDestroy(select_ray);
		select_ray = 0;
		select_finish = FALSE;
	}
}
