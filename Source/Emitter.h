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

#ifndef EMITTER_H
#define EMITTER_H

#include <QList>
#include <QString>
#include "Vector3.h"
#include "Particle.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Texture;
class Particle;

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Emitter
{
public:
	// Constructors
	 Emitter			(QString filename);
	~Emitter			(void);

	// Update & Draw
	void Update			(quint32 gametime);
	void Draw			(quint32 gametime);

	// Accessors
	const Texture *Tex	(void) const;

	QString Name		(void) const;
	bool Loaded			(void) const;

protected:
	// Internal
	void Reset			(void);
	void AddParticle	(void);
	bool Load			(QString filename);

protected:
	// Variables
	QList<Particle *>	mParticles;
	Texture				*mTexture;

	quint32				mLastTime;

	QString	mName;
	bool	mLoaded;

public:
	// Attributes
	float	Rate;
	Vector3	Radius;

	float	LifeMin;
	float	LifeMax;

	float	SizeMin;
	float	SizeMax;

	float	Saturation;
	Vector3	Color;
	float	Alpha;

	Vector3	Spread;
	float	Gravity;

	Vector3	Position;
	Vector3	Rotation;
	Vector3	Wind;

	float	Bounciness;
};

#endif // EMITTER_H
