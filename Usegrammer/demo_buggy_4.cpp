/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/*

buggy with suspension.
this also shows you how to use geom groups.

*/


#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

// select correct drawing functions

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif


// some constants

#define LENGTH 0.7	// chassis length
#define WIDTH 0.5	// chassis width
#define HEIGHT 0.2	// chassis height
#define RADIUS 0.18	// wheel radius
#define STARTZ 0.5	// starting height of chassis
#define CMASS 1		// chassis mass
#define WMASS 0.2	// wheel mass

static const dVector3 yunit = { 0, 1, 0 }, zunit = { 0, 0, 1 };


// dynamics and collision objects (chassis, 3 wheels, environment)

static dWorldID world;
static dSpaceID space;
static dBodyID body[8];  // 4 pieces of body = box + 3 wheels; need to be encapsulated in cart ?
static dJointID joint[3];	// joint[0] is the front wheel
static dJointGroupID contactgroup;
static dGeomID ground;
static dSpaceID car_space;
static dGeomID box[2];  // box = cart ?
static dGeomID sphere[6]; // C * 3; each for one wheel
static dGeomID ground_box;


// things that the user controls

static dReal speed=0,steer=0;	// user commands


// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

static void nearCallback (void *, dGeomID o1, dGeomID o2)
{
  int i,n;

  // only collide things with the ground
  int g1 = (o1 == ground || o1 == ground_box);
  int g2 = (o2 == ground || o2 == ground_box);
  if (!(g1 ^ g2)) return;

  const int N = 10;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
	dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.3;
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      dJointAttach (c,
		    dGeomGetBody(contact[i].geom.g1),
		    dGeomGetBody(contact[i].geom.g2));
    }
  }
}

#include <stack>
#include <string>
#include <iostream>
#include <initializer_list>
#include <typeinfo>
#include <type_traits>
#include <bits/stdc++.h>

// Phos Stack Machine 2020-08-06
std::stack<void*> sm_S;
std::stack<std::string> sm_SS;

void *sm_pop()
{
  void *t = sm_S.top();
  sm_S.pop();
  
  return t;

}

void sm_svp() // set view point
{

  float *fa=(float*)sm_pop();
  printf("\n\nIn sm_svp\nsm_S: %f\n", fa[0]);
  
  float *fb=(float*)sm_pop();
  printf("sm_S: %f\n", fb[0]);

  // read stack value is fine. but must preserve with variable?

  // dsSetViewpoint ( (float*)sm_pop(), (float*)sm_pop());
  // dsSetViewpoint ( (static float*)sm_pop(), (static float*)sm_pop());
  
  dsSetViewpoint (fa,fb);
  
}


void sm_dJointSetHinge2Param() // dJointSetHinge2Param
{

  // get function prototype of target function
  // use type definition from prototype
  // work backwards on push
  
  // OpenDynamicsEngine/ode-0.16.2/include/ode$ grep -i -n dJointSetHinge2Param *
  // objects.h:2002:ODE_API void dJointSetHinge2Param (dJointID, int parameter, dReal value);

  float *fa=(float*)sm_pop();
  printf("\n\nIn sm_svp\nsm_S: %f\n", fa[0]);
  
  float *fb=(float*)sm_pop();
  printf("sm_S: %f\n", fb[0]);
  
  // joint[0],dParamVel2,-speed

  // read stack value is fine. but must preserve with variable?

  // dsSetViewpoint ( (float*)sm_pop(), (float*)sm_pop());
  // dsSetViewpoint ( (static float*)sm_pop(), (static float*)sm_pop());
  //dsSetViewpoint (fa,fb);
  
  dJointSetHinge2Param (joint[0],dParamVel2,-speed);  
}


void foo() { std::cout << "foo()"; }
void boo() { std::cout << "boo()"; }
void too() { std::cout << "too()"; }
void goo() { std::cout << "goo()"; }

int map_func(std::string func) {
  std::map<std::string, std::function<void()>> functions;
  functions["foo"] = foo;
  functions["boo"] = boo;
  functions["too"] = too;
  functions["goo"] = goo;
  functions["svp:"] = sm_svp;
  functions[ "dJointSetHinge2Param:" ] = sm_dJointSetHinge2Param;

  // std::string func;
  // std::cin >> func;
  if (functions.find(func) != functions.end()) {
    functions[func]();
  }
  return 0;
}


template <typename T>
using is_string = std::is_constructible<std::string, T>;

template <typename T> const char* sm_typeof(T&) { return "unknown"; }    // default
template<> const char* sm_typeof(int&) { return "int"; }
template<> const char* sm_typeof(float&) { return "float"; }
template<> const char* sm_typeof(std::string&) { return "string"; }
template<> const char* sm_typeof(float*&) { return "ptr_float"; }
template<> const char* sm_typeof(void*&) { return "ptr_void"; }
template<> const char* sm_typeof(char*&) { return "ptr_char"; }


// template functions sm_lastchar
template <typename T> const char* sm_lastchar(T& t) { 
std::cout << "    DEFAULT sm_lastchar " << t << "  is_string " << is_string<decltype(t)>::value <<std::endl;
return "unknown "; } 

template<> const char* sm_lastchar(int& t) { 
std::cout << "    int sm_lastchar " <<  t <<std::endl;
return "unknown "; } 

template<> const char* sm_lastchar(dReal& t) { 
std::cout << "    dReal sm_lastchar " <<  t <<std::endl;
return "unknown "; } 


template<> const char* sm_lastchar(dJointID& t) { 
std::cout << "    dJointID sm_lastchar " <<  t <<std::endl;
return "unknown "; } 

/*
template<> const char* sm_lastchar(dParamVel2& t) { 
std::cout << "    dParamVel2 sm_lastchar " <<  t <<std::endl;
return "unknown "; } 
*/

template<> const char* sm_lastchar(std::string& t) 
{ 
    std::cout << "    sm_lastchar " << t.back() <<std::endl;
    
    if (t.back()==':') {
        std::cout << "    is colon " << t.back() <<std::endl;

        map_func(t);
    }
    return "string "+t.back(); 
}

template<> const char* sm_lastchar(float*& t) { 
    sm_S.push((void*)t);
    return "ptr_float"; 
}

// const char* sm_lastchar(std::string&) { return s.back(); }
// const char* sm_lastchar(std::string&) { return s.back(); }

template <typename T>
void funcvar(T t) 
{
    // std::cout << t << std::endl ;
    
    std::cout << t << ' ' ;
    std::cout << typeid(t).name() << ' ' ;
    std::cout << std::is_function<decltype(t)>::value << ' ';
    std::cout << sm_typeof(t) <<std::endl  ;
}

template<typename T, typename... Args>
void funcvar(T t, Args... args) // recursive variadic function
{
    std::cout << t << ' ' ;
    std::cout << typeid(t).name() << ' ' ;
    std::cout << std::is_function<decltype(t)>::value << ' ';
    std::cout << sm_typeof(t) <<std::endl  ;

    funcvar(args...) ;
}




// sm_proc recursive variadic 
template <typename T>
void sm_proc(T t) 
{
    // std::cout << t << std::endl ;
    
    std::cout << t << ' ' ;
    std::cout << typeid(t).name() << ' ' ;
    std::cout << std::is_function<decltype(t)>::value << ' ';
    std::cout << "    is_string " << is_string<decltype(t)>::value << ' ';
    std::cout << sm_typeof(t) <<std::endl  ;
    
    if ( (is_string<decltype(t)>::value)==1 ) {
      // std::cout << "    last char " << t.back() <<std::endl  ;
      // std::cout << "    last char "  <<std::endl  ;
      std::string s_t(t);
      std::cout << "    s_t " << s_t << "    last char " << sm_lastchar(s_t) <<std::endl  ;
    }
    else sm_lastchar(t); // sm_S.push(t);
}



template<typename T, typename... Args>
void sm_proc(T t, Args... args) // recursive variadic function
{
    std::cout << t << ' ' ;
    std::cout << typeid(t).name() << ' ' ;
    std::cout << std::is_function<decltype(t)>::value << ' ';
    std::cout << "    is_string " << is_string<decltype(t)>::value << ' ';
    std::cout << sm_typeof(t) <<std::endl  ;

    // if (1) {
    if ( (is_string<decltype(t)>::value)==1 ) {
      // std::cout << "    last char " << t.back() <<std::endl  ;
      // std::cout << "    last char "  <<std::endl  ;
      std::cout << "    last char " << sm_lastchar(t) <<std::endl  ;
    }
    else sm_lastchar(t); // sm_S.push(t);

    sm_proc(args...) ;
}

int assc_arr(){

using namespace std;

   map<string, int> speed{ { "ninja", 290 },
   { "s1000rr", 310 }, { "bullet", 127 },
   { "Duke", 135 }, { "R1", 286 } };
   map<string, int>::iterator i;
   cout << "The topspeed of bikes are" << endl;
   for (i = speed.begin(); i != speed.end(); i++)
   cout<<i->first<<" "<<i->second <<endl;
   cout << endl;
   cout << "The top speed of bullet is "<< speed["bullet"] << endl;
}



// #include <iostream>
int test_string()
{
    std::cout << std::boolalpha
        << is_string<const char*>::value << "\n"
        << is_string<volatile char*>::value << "\n"
        << is_string<std::string>::value << "\n"
        ;
}

// start simulation - set viewpoint

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);

  static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
  static float hpr[3] = {121.0000f,-27.5000f,0.0000f};


  // 2020-08-08 Test code for recursive variadic function  
      std::string
        str1( "Hello" ),
        str2( "world" );

      funcvar(1,2.5,'a',str1);
    

  // 2020-08-08 Phos code
  sm_proc( hpr, xyz, "svp:" );

  
  // 2020-08-08 original ODE code
  // dsSetViewpoint (xyz,hpr);
  
  printf ("Press:\t'a' to increase speed.\n"
	  "\t'z' to decrease speed.\n"
	  "\t',' to steer left.\n"
	  "\t'.' to steer right.\n"
	  "\t' ' to reset speed and steering.\n"
	  "\t'1' to save the current state to 'state.dif'.\n");
}


// called when a key pressed

static void command (int cmd)
{
  switch (cmd) {
  case 'a': case 'A':
    speed += 0.3;
    break;
  case 'z': case 'Z':
    speed -= 0.3;
    break;
  case ',':
    steer -= 0.5;
    break;
  case '.':
    steer += 0.5;
    break;
  case ' ':
    speed = 0;
    steer = 0;
    break;
  case '1': {
      FILE *f = fopen ("state.dif","wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
    }
  }
}


// simulation loop

static void simLoop (int pause)
{
  int i;
  int SIM_JOINT=0;
  
  if (!pause) {
    // motor
    
    // sm_proc( joint[0],dParamVel2,-speed, "3 reverse_param: dJointSetHinge2Param:" )
    // sm_proc( -speed, dParamVel2, joint[0], "dJointSetHinge2Param:" );
    
    if (SIM_JOINT>0) {
    dJointSetHinge2Param (joint[0],dParamVel2,-speed);

    dJointSetHinge2Param (joint[0],dParamFMax2,0.1);

    // preserve order of parameters, then reverse them in Phos, to minimize code change

    // sm_proc( joint[0],dParamVel2,-speed, "3 reverse_param: dJointSetHinge2Param:" );

    // steering
    dReal v = steer - dJointGetHinge2Angle1 (joint[0]);
    if (v > 0.1) v = 0.1;
    if (v < -0.1) v = -0.1;
    v *= 10.0;
    dJointSetHinge2Param (joint[0],dParamVel,v);
    dJointSetHinge2Param (joint[0],dParamFMax,0.2);
    dJointSetHinge2Param (joint[0],dParamLoStop,-0.75);
    dJointSetHinge2Param (joint[0],dParamHiStop,0.75);
    dJointSetHinge2Param (joint[0],dParamFudgeFactor,0.1);
    }

    dSpaceCollide (space,0,&nearCallback);
    dWorldStep (world,0.05);

    // remove all contact joints
    dJointGroupEmpty (contactgroup);

  }

  dsSetColor (0,1,1);
  dsSetTexture (DS_WOOD);
  dReal sides[3] = {LENGTH,WIDTH,HEIGHT};


  dsDrawBox (dBodyGetPosition(body[0]),dBodyGetRotation(body[0]),sides);

  int C=1;
  if (C>0)
  dsDrawBox (dBodyGetPosition(body[4*C+0]),dBodyGetRotation(body[4*C+0]),sides);


  dsSetColor (1,1,1);
  for (i=1; i<=3; i++) dsDrawCylinder (dBodyGetPosition(body[i]),
				       dBodyGetRotation(body[i]),0.02f,RADIUS);

  if (C>0)
  for (i=1; i<=3; i++) dsDrawCylinder (dBodyGetPosition(body[4*C+i]),
				       dBodyGetRotation(body[4*C+i]),0.02f,RADIUS);


  dVector3 ss;
  dGeomBoxGetLengths (ground_box,ss);
  dsDrawBox (dGeomGetPosition(ground_box),dGeomGetRotation(ground_box),ss);

}


int sm_buggy(dMass *pm, float X, float Y, int C) // C = cart index
{
  dMass m;

  // Phos 2020-0812
  // body = box + 3 sphere, has mass

  // chassis body
  body[4*C+0] = dBodyCreate (world);
  dBodySetPosition (body[4*C+0],X,Y,STARTZ);
  dMassSetBox (pm,1,LENGTH,WIDTH,HEIGHT);
  dMassAdjust (pm,CMASS);
  dBodySetMass (body[4*C+0],pm);
  box[C] = dCreateBox (0,LENGTH,WIDTH,HEIGHT);
  dGeomSetBody (box[C],body[4*C+0]);

  // wheel bodies
  int i;
  for (i=1; i<=3; i++) {
    body[4*C+i] = dBodyCreate (world);
    dQuaternion q;
    dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
    dBodySetQuaternion (body[4*C+i],q);
    dMassSetSphere (pm,1,RADIUS);
    dMassAdjust (pm,WMASS);
    dBodySetMass (body[4*C+i],pm);
    sphere[4*C+i-1] = dCreateSphere (0,RADIUS);
    dGeomSetBody (sphere[4*C+i-1],body[4*C+i]);
  }
  dBodySetPosition (body[4*C+1],0.5*LENGTH+X,0+Y,STARTZ-HEIGHT*0.5);
  dBodySetPosition (body[4*C+2],-0.5*LENGTH+X, WIDTH*0.5+Y,STARTZ-HEIGHT*0.5);
  dBodySetPosition (body[4*C+3],-0.5*LENGTH+X,-WIDTH*0.5+Y,STARTZ-HEIGHT*0.5);
}


int main (int argc, char **argv)
{
  int i;
  dMass m, m1;

  // setup pointers to drawstuff callback functions
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = &command;
  fn.stop = 0;
  fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

  // create world
  dInitODE2(0);
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactgroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,0,-0.5);
  ground = dCreatePlane (space,0,0,1,0);


  if (1) { 
  
    sm_buggy(&m1,2,2,1);
    sm_buggy(&m,0,0,0);
  
  
  }
  else {

  // chassis body
  body[0] = dBodyCreate (world);
  dBodySetPosition (body[0],0,0,STARTZ);
  dMassSetBox (&m,1,LENGTH,WIDTH,HEIGHT);
  dMassAdjust (&m,CMASS);
  dBodySetMass (body[0],&m);
  box[0] = dCreateBox (0,LENGTH,WIDTH,HEIGHT);
  dGeomSetBody (box[0],body[0]);

  // wheel bodies
  for (i=1; i<=3; i++) {
    body[i] = dBodyCreate (world);
    dQuaternion q;
    dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
    dBodySetQuaternion (body[i],q);
    dMassSetSphere (&m,1,RADIUS);
    dMassAdjust (&m,WMASS);
    dBodySetMass (body[i],&m);
    sphere[i-1] = dCreateSphere (0,RADIUS);
    dGeomSetBody (sphere[i-1],body[i]);
  }
  dBodySetPosition (body[1],0.5*LENGTH,0,STARTZ-HEIGHT*0.5);
  dBodySetPosition (body[2],-0.5*LENGTH, WIDTH*0.5,STARTZ-HEIGHT*0.5);
  dBodySetPosition (body[3],-0.5*LENGTH,-WIDTH*0.5,STARTZ-HEIGHT*0.5);
  }
  
  int SIM_JOINT=0;
  
  if (SIM_JOINT>0) {
  // front and back wheel hinges
  for (i=0; i<3; i++) {
    joint[i] = dJointCreateHinge2 (world,0);
    dJointAttach (joint[i],body[0],body[i+1]);
    const dReal *a = dBodyGetPosition (body[i+1]);
    dJointSetHinge2Anchor (joint[i],a[0],a[1],a[2]);
    dJointSetHinge2Axes (joint[i], zunit, yunit);
  }

  // set joint suspension
  for (i=0; i<3; i++) {
    dJointSetHinge2Param (joint[i],dParamSuspensionERP,0.4);
    dJointSetHinge2Param (joint[i],dParamSuspensionCFM,0.8);
  }

  // lock back wheels along the steering axis
  for (i=1; i<3; i++) {
    // set stops to make sure wheels always stay in alignment
    dJointSetHinge2Param (joint[i],dParamLoStop,0);
    dJointSetHinge2Param (joint[i],dParamHiStop,0);
    // the following alternative method is no good as the wheels may get out
    // of alignment:
    //   dJointSetHinge2Param (joint[i],dParamVel,0);
    //   dJointSetHinge2Param (joint[i],dParamFMax,dInfinity);
  }
  
  }

  // create car space and add it to the top level space
  car_space = dSimpleSpaceCreate (space);
  
  // car_space = dHashSpaceCreate(0);
  
  dSpaceSetCleanup (car_space,0);
  dSpaceAdd (car_space,box[0]);
  dSpaceAdd (car_space,sphere[0]);
  dSpaceAdd (car_space,sphere[1]);
  dSpaceAdd (car_space,sphere[2]);

  // Phos 2020-08-12
  dSpaceAdd (car_space,box[1]);
  // use sphere because wheel turn = sphere?
  // dSpaceAdd (car_space,sphere[3]);
  dSpaceAdd (car_space,sphere[4]);
  dSpaceAdd (car_space,sphere[5]);
  
  
  // environment
  ground_box = dCreateBox (space,2,1.5,1);
  dMatrix3 R;
  dRFromAxisAndAngle (R,0,1,0,-0.15);
  dGeomSetPosition (ground_box,2,0,-0.34);
  dGeomSetRotation (ground_box,R);

  // drawstuff is initiated only in dsSimulationLoop
  // all functions before this only set up d* dynamics

  // run simulation
  dsSimulationLoop (argc,argv,352,288,&fn);

  dGeomDestroy (box[0]);
  dGeomDestroy (sphere[0]);
  dGeomDestroy (sphere[1]);
  dGeomDestroy (sphere[2]);
  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}
