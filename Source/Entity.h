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

#ifndef ENTITY_H
#define ENTITY_H

#include <QList>
#include <QString>

#include "Model.h"
#include "Random.h"
#include "Emitter.h"
#include "Texture.h"
#include "Vector3.h"
#include "SpriteSheet.h"

#define SAFE_DELETE( x ) if (x != NULL) { delete x; x = NULL; }



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents an abstract game entity. </summary>

class Entity
{
public:
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Represents a type of entity. </summary>

	enum EType
	{
		ePea,
		eSun,
		eGeode,
		eGrunt,
		eWalnut,
		eUrchin,
		eSunflower,
		eCherrybomb,
		ePeashooter,
	};

public:
	// Constructors
	Entity (Vector3 position = Vector3 (0, 0, 0),
			Vector3 rotation = Vector3 (0, 0, 0),
			Vector3 scale    = Vector3 (1, 1, 1));

	virtual ~Entity (void);

	// Accessors
	virtual QString Name		(void) const = 0;
	virtual Entity::EType Type	(void) const = 0;

	// Update & Draw
	virtual void Update	(quint32 gametime) { };
	virtual void Draw	(quint32 gametime) { };

	// Utility
	float Distance (const Entity  *target) const;
	float Distance (const Vector3 &target) const;

public:
	// Variables
	qreal	Health;		// Entity health

	Vector3 Position;	// Entity position
	Vector3 Rotation;	// Entity rotation
	Vector3 Scale;		// Entity scale

	static QList<Entity *> Entities;
};

#endif // ENTITY_H
