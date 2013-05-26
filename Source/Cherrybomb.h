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

#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include "Entity.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Cherrybomb : public Entity
{
public:
	//----------------------------------------------------------------------------//
	// Constructors                                                               //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Cherrybomb (Vector3 position = Vector3()) : Entity (position) { }



	//----------------------------------------------------------------------------//
	// Accessors                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the name of this entity. </summary>

	virtual QString Name (void) const { return "Cherrybomb"; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the type of this entity. </summary>

	virtual Entity::EType Type (void) const { return Entity::eCherrybomb; }



	//----------------------------------------------------------------------------//
	// Update & Draw                                                              //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Updates this entity. </summary>

	virtual void Update	(quint32 gametime)
	{
		Scale.X *= 1.015f;
		Scale.Y *= 1.010f;
		Scale.Z *= 1.015f;
		if (Scale.X > 2.0f)
		{
			for (qint32 i = 0; i < Entities.size(); ++i)
				if ((Entities.at(i)->Type() == Entity::eGeode ||
					 Entities.at(i)->Type() == Entity::eGrunt ||
					 Entities.at(i)->Type() == Entity::eUrchin) &&
					(Entity::Entities.at(i)->Position.X < Position.X + 20.0f &&
					 Entity::Entities.at(i)->Position.X > Position.X - 20.0f) &&
					(Entity::Entities.at(i)->Position.Z < Position.Z + 20.0f &&
					 Entity::Entities.at(i)->Position.Z > Position.Z - 20.0f))
					 Entities.at(i)->Health -= 130.0f;

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
			mModel = new Model ("Content/Cherrybomb.fbx");
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
};

#endif // CHERRYBOMB_H
