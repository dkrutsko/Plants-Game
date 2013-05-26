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

#ifndef URCHIN_H
#define URCHIN_H

#include "Game.h"
#include "Entity.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Urchin : public Entity
{
public:
	//----------------------------------------------------------------------------//
	// Constructors                                                               //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Urchin (Vector3 position = Vector3()) : Entity (position)
	{
		mLastAttack = 0;
		Health = Random (180, 200);
	}



	//----------------------------------------------------------------------------//
	// Accessors                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the name of this entity. </summary>

	virtual QString Name (void) const { return "Urchin"; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the type of this entity. </summary>

	virtual Entity::EType Type (void) const { return Entity::eUrchin; }



	//----------------------------------------------------------------------------//
	// Update & Draw                                                              //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Updates this entity. </summary>

	virtual void Update	(quint32 gametime)
	{
		Entity *lTarget = NULL;
		for (qint32 i = 0; i < Entity::Entities.size(); ++i)
			if ((Entity::Entities.at(i)->Type() == Entity::eWalnut ||
				 Entity::Entities.at(i)->Type() == Entity::ePeashooter ||
				 Entity::Entities.at(i)->Type() == Entity::eSunflower) &&
				(Entity::Entities.at(i)->Position.X < Position.X + 8.0f &&
				 Entity::Entities.at(i)->Position.X > Position.X - 8.0f) &&
				(Entity::Entities.at(i)->Position.Z < Position.Z + 2.0f &&
				 Entity::Entities.at(i)->Position.Z > Position.Z - 2.0f))
			{
				lTarget = Entity::Entities.at(i);
				break;
			}

		if (lTarget == Game::Selected())
			lTarget = NULL;

		if (lTarget == NULL)
		{
			Rotation.Y = 0.0f;
			Position.X -= 0.04f;
		}

		else
		{
			Rotation.Y -= 5.0f;
			if (gametime - mLastAttack > 200)
			{
				lTarget->Health -= 17;
				mLastAttack = gametime;
			}
		}

		if (Position.X < -55.0)
			Game::Lost();

		if (Health < 0)
			delete this;
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
			mModel = new Model ("Content/Urchin.fbx");
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
	quint32 mLastAttack;
};

#endif // URCHIN_H
