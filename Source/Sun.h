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

#ifndef SUN_H
#define SUN_H

#include "Entity.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Sun : public Entity
{
public:
	//----------------------------------------------------------------------------//
	// Constructors                                                               //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Sun (Vector3 position = Vector3()) : Entity (position)
	{
		Position.Y = 150.0f;

		Falling  = true;
		Selected = false;

		mEmitter = new Emitter ("Content/Sun.xml");
		++mSunCount;
	}

	~Sun (void)
	{
		delete mEmitter;
		--mSunCount;
	}



	//----------------------------------------------------------------------------//
	// Accessors                                                                  //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the name of this entity. </summary>

	virtual QString Name (void) const { return "Sun"; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Returns the type of this entity. </summary>

	virtual Entity::EType Type (void) const { return Entity::eSun; }

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> </summary>

	static quint32 SunCount (void) { return mSunCount; }



	//----------------------------------------------------------------------------//
	// Update & Draw                                                              //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Updates this entity. </summary>

	virtual void Update	(quint32 gametime)
	{
		if (Falling && !Selected)
		{
			Position.Y -= 1.50f;
			if (Position.Y < 0.0f)
			{
				Falling = false;
				Position.Y = 0.0f;
			}
		}

		else if (Selected)
		{
			Position.Y -= 0.25f;
			if (Position.Y < -8.0f)
			{
				delete this;
				return;
			}
		}

		if (Position.Y < 36.0f)
			mEmitter->Wind.Y = 14.0f;

		mEmitter->Position = Position;
		mEmitter->Position.Y += 2.0;
		mEmitter->Update (gametime);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Draws this entity. </summary>

	virtual void Draw (quint32 gametime)
	{
		if (mModel != NULL)
			mModel->Draw (gametime, Position, Rotation, Scale);

		mEmitter->Draw (gametime);
	}



	//----------------------------------------------------------------------------//
	// Static                                                                     //
	//----------------------------------------------------------------------------//

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Preloads any required static content. </summary>

	static void PreloadContent (void)
	{
		if (mModel == NULL)
			mModel = new Model ("Content/Sun.fbx");
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
	static quint32 mSunCount;
	Emitter *mEmitter;

public:
	bool Falling;
	bool Selected;
};

#endif // SUN_H
