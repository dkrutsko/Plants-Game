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

#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "Pea.h"
#include "Entity.h"

#include "Geode.h"
#include "Grunt.h"
#include "Urchin.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Peashooter : public Entity
{
public:
	//----------------------------------------------------------------------------//
	// Constructors                                                               //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Peashooter (Vector3 position = Vector3()) : Entity (position)
	{
		mLastTime = 0;
		mNextTime = 1500;

		Health = 160;
	}



	//----------------------------------------------------------------------------//
	// Accessors                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the name of this entity. </summary>

	virtual QString Name (void) const { return "Peashooter"; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the type of this entity. </summary>

	virtual Entity::EType Type (void) const { return Entity::ePeashooter; }



	//----------------------------------------------------------------------------//
	// Update & Draw                                                              //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Updates this entity. </summary>

	virtual void Update	(quint32 gametime)
	{
		if (Health < 0)
		{
			delete this;
			return;
		}

		if (mLastTime == 0)
			mLastTime = gametime;

		bool lNewPea = false;
		for (qint32 i = 0; i < Entity::Entities.size(); ++i)
			if ((Entity::Entities.at(i)->Type() == Entity::eGeode ||
				 Entity::Entities.at(i)->Type() == Entity::eGrunt ||
				 Entity::Entities.at(i)->Type() == Entity::eUrchin) &&
				(Entity::Entities.at(i)->Position.Z < Position.Z + 2.0f &&
				 Entity::Entities.at(i)->Position.Z > Position.Z - 2.0f))
			{
				lNewPea = true;
				break;
			}

		if (lNewPea && gametime - mLastTime > mNextTime)
		{
			new Pea (Position);
			mLastTime = gametime;
		}

		if (Health < 160)
			Health += 1;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Draws this entity. </summary>

	virtual void Draw (quint32 gametime)
	{
		if (mModel != NULL)
			mModel->Draw (gametime, Position, Rotation, Scale);
	}



	//----------------------------------------------------------------------------//
	// Static                                                                     //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Preloads any required static content. </summary>

	static void PreloadContent (void)
	{
		if (mModel == NULL)
			mModel = new Model ("Content/Peashooter.fbx");
	}

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Unloads statically loaded content. </summary>

	static void UnloadContent (void)
	{
		SAFE_DELETE (mModel)
	}

protected:
	// Variables
	static Model *mModel;

	quint32 mLastTime;
	quint32 mNextTime;
};

#endif // PEASHOOTER_H
