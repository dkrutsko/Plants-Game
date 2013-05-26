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

#ifndef INPUT_H
#define INPUT_H

#include <QGlobal.h>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Keyboard
{
public:
	// Constructors
	Keyboard (void);

	// Static
	static Keyboard *State (void);

	// Operators
	bool operator [] (quint32 key) const;

private:
	// Variables
	quint8 *mKeys;	// Keys

	static Keyboard mState;
};

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Mouse
{
public:
	// Constructors
	Mouse (void);

	// Accessors
	quint32 X	(void) const;
	quint32 Y	(void) const;
	bool Left	(void) const;
	bool Middle	(void) const;
	bool Right	(void) const;

	// Static
	static Mouse *State (void);

private:
	// Variables
	quint32 mX;		// X position
	quint32 mY;		// Y position

	bool mLeft;		// Left button down
	bool mMiddle;	// Middle button down
	bool mRight;	// Right button down

	static Mouse mState;
};

#endif // INPUT_H
