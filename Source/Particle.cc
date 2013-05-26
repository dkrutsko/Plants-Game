////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                        (C) 2011-2013  David Krutsko                        //
//                        See LICENSE.md for copyright                        //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include <cmath>

#define STRICT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"



//----------------------------------------------------------------------------//
// Static                                                            Particle //
//----------------------------------------------------------------------------//

float Particle::RotateX = 0.0f;
float Particle::RotateY = 0.0f;



//----------------------------------------------------------------------------//
// Constructors                                                      Particle //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Particle::Particle (qint32 pID)
{
	mID = pID;
	Reset();
}



//----------------------------------------------------------------------------//
// Update & Draw                                                     Particle //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Particle::Update (quint32 pGametime)
{
	if (!mActive) return;
	if (mLastTime == 0)
		mLastTime = pGametime;

	float lChange = (float) (pGametime - mLastTime) / 1000.0f;

	Velocity += Acceleration	* lChange;
	Position += Velocity		* lChange;

//	if (Position.Y < 0.0f)
//	{
//		Velocity.Y = Velocity.Y * -Bounciness;
//		Position.Y = 0.0f;
//	}

	Life -= lChange;
	if (Life <= 0.0f)
		mActive = false;

	mLastTime = pGametime;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Particle::Draw (quint32 pGametime)
{
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();

	float lX = Position.X;
	float lY = Position.Y;
	float lZ = Position.Z;

	Rotate (Rotation.X * pGametime / 1000.0f, lY, lZ);
	Rotate (Rotation.Y * pGametime / 1000.0f, lX, lZ);
	Rotate (Rotation.Z * pGametime / 1000.0f, lX, lY);

	const float lFadeinTime		= 0.2f;
	const float lFadeoutTime	= 0.7f;

	if (TotalLife - Life < lFadeinTime)
		glColor4f (Color.X, Color.Y, Color.Z,
			(TotalLife - Life) / lFadeinTime * Alpha);

	else if (Life < lFadeoutTime)
		glColor4f (Color.X, Color.Y, Color.Z,
			Life / lFadeoutTime * Alpha);

	else glColor4f (Color.X, Color.Y, Color.Z, Alpha);

	glTranslatef (lX, lY, lZ);

	// Always face the camera
	glRotatef (-RotateY, 0, 1, 0);
	glRotatef (-RotateX, 1, 0, 0);
		// This is sort of a hack...

//	if (mID % 5 == 0)
//		 glRotatef (Life *  100.0f, 0, 0, 1);
//	else glRotatef (Life * -100.0f, 0, 0, 1);

	glScalef (Size, Size, Size);

	if (mList == 0)
	{
		mList = glGenLists(1);
		glNewList (mList, GL_COMPILE_AND_EXECUTE);

		glBegin (GL_TRIANGLE_STRIP);
			glTexCoord2f (1, 1); glVertex3f ( 1,  1, 0);
			glTexCoord2f (0, 1); glVertex3f (-1,  1, 0);
			glTexCoord2f (1, 0); glVertex3f ( 1, -1, 0);
			glTexCoord2f (0, 0); glVertex3f (-1, -1, 0);
		glEnd();

		glEndList();
	}

	else glCallList (mList);

	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
}



//----------------------------------------------------------------------------//
// Accessors                                                         Particle //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

qint32 Particle::ID (void) const
{
	return mID;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Particle::Active (void) const
{
	return mActive;
}



//----------------------------------------------------------------------------//
// Internal                                                          Particle //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Particle::Reset (void)
{
	mActive			= true;
	mLastTime		= 0;
	mList			= 0;

	Position		= Vector3();
	Rotation		= Vector3();
	Velocity		= Vector3();
	Acceleration	= Vector3();
	
	Color			= Vector3();

	TotalLife		= 1.0f;
	Life			= 1.0f;

	Alpha			= 1.0f;
	Size			= 1.0f;

	Ground			= 0.0f;
	Bounciness		= 0.9f;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Particle::Rotate (float pAngle, float &pX, float &pY)
{
	float lX = pX * cos (pAngle) - pY * sin (pAngle);
	float lY = pY * cos (pAngle) - pX * sin (pAngle);

	pX = lX;
	pY = lY;
}
