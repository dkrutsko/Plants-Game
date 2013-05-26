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

#ifndef GRUNT_H
#define GRUNT_H

#include "Game.h"
#include "Entity.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Grunt : public Entity
{
public:
	//----------------------------------------------------------------------------//
	// Constructors                                                               //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Grunt (Vector3 position = Vector3()) : Entity (position)
	{
		mLastAttack = 0;
		mDirection = true;
		Health = Random (280, 320);
	}



	//----------------------------------------------------------------------------//
	// Accessors                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the name of this entity. </summary>

	virtual QString Name (void) const { return "Grunt"; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the type of this entity. </summary>

	virtual Entity::EType Type (void) const { return Entity::eGrunt; }



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
			mDirection = true;
			Position.Y = 0.0f;
			Position.X -= 0.02f;
		}

		else
		{
			if (mDirection)
				 Position.Y += 0.1f;
			else Position.Y -= 0.4f;

			if (Position.Y > 5.0f) mDirection = false;
			if (Position.Y < 0.0f) mDirection = true;

			if (gametime - mLastAttack > 400)
			{
				lTarget->Health -= 35;
				mLastAttack = gametime;
			}
		}

		if (Position.X < -55.0)
			Game::Lost();

		if (Health < 0)
		{
			Game::Won();
			delete this;
		}
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
			mModel = new Model ("Content/Grunt.fbx");
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
	bool mDirection;
};

#endif // GRUNT_H
