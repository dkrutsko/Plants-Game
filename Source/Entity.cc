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
#include "Entity.h"

#include "Pea.h"
#include "Sun.h"
#include "Geode.h"
#include "Grunt.h"
#include "Walnut.h"
#include "Urchin.h"
#include "Sunflower.h"
#include "Cherrybomb.h"
#include "Peashooter.h"



//----------------------------------------------------------------------------//
// Static                                                              Entity //
//----------------------------------------------------------------------------//

QList<Entity *> Entity::Entities;

// Other Entities
Model *Pea			:: mModel = NULL;
Model *Sun			:: mModel = NULL;
Model *Geode		:: mModel = NULL;
Model *Grunt		:: mModel = NULL;
Model *Walnut		:: mModel = NULL;
Model *Urchin		:: mModel = NULL;
Model *Sunflower	:: mModel = NULL;
Model *Cherrybomb	:: mModel = NULL;
Model *Peashooter	:: mModel = NULL;

quint32 Sun:: mSunCount = 0;



//----------------------------------------------------------------------------//
// Constructors                                                        Entity //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Entity::Entity (Vector3 pPosition, Vector3 pRotation, Vector3 pScale)
{
	Position	= pPosition;
	Rotation	= pRotation;
	Scale		= pScale;

	Entities << this;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Entity::~Entity (void)
{
	Entities.removeAt (Entities.indexOf (this));
}



//----------------------------------------------------------------------------//
// Utility                                                              Entity //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

float Entity::Distance (const Entity *pTarget) const
{
	Vector3 lTemp = pTarget->Position - Position;

	return sqrt (lTemp.X * lTemp.X +
				 lTemp.Y * lTemp.Y +
				 lTemp.Z * lTemp.Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

float Entity::Distance (const Vector3 &pTarget) const
{
	Vector3 lTemp = pTarget - Position;

	return sqrt (lTemp.X * lTemp.X +
				 lTemp.Y * lTemp.Y +
				 lTemp.Z * lTemp.Z);
}
