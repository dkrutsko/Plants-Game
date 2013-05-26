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

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"
#include <QGlobal.h>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Particle
{
public:
	// Constructors
	Particle		(qint32 id);

	// Update & Draw
	void Update		(quint32 gametime);
	void Draw		(quint32 gametime);

	// Accessors
	qint32 ID		(void) const;
	bool   Active	(void) const;

protected:
	// Internal
	void Reset		(void);
	void Rotate		(float angle, float &x, float &y);

protected:
	// Variables
	qint32		mID;
	bool		mActive;

	quint32		mLastTime;
	quint32		mList;

public:
	Vector3		Position;
	Vector3		Rotation;
	Vector3		Velocity;
	Vector3		Acceleration;

	Vector3		Color;
	
	float		TotalLife;
	float		Life;

	float		Alpha;
	float		Size;

	float		Ground;
	float		Bounciness;

	static float RotateX;
	static float RotateY;
};

#endif // PARTICLE_H
