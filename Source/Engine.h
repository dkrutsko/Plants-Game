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

#ifndef ENGINE_H
#define ENGINE_H

#include <QGlobal.h>

////////////////////////////////////////////////////////////////////////////////
/// <summary> Defines the default window dimensions. </summary>

#define WIDTH	1680
#define HEIGHT	1050



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Game;
class QSize;
class Vector3;

////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes and updates the game. </summary>

class Engine
{
public:
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> </summary>

	enum FillMode
	{
		Solid,
		Line,
		Point,
	};

public:
	// Engine
	int Run				(void);

	void SetMode2D		(void);
	void SetMode3D		(void);

	void Exit			(void);
	
	int Width			(void) const;
	int Height			(void) const;
	QSize Size			(void) const;

	FillMode GetFillMode(void) const;
	void SetFillMode	(FillMode mode);

	void MakeBetter		(void);
	void MakeCoffee		(void);

private:
	// Internal
	void Initialize		(void);
	void Uninitialize	(void);
	void UpdateFPS		(void);

private:
	// Variables
	qint32 mWidth;		// Window width
	qint32 mHeight;		// Window height
	bool mExit;			// Whether to exit

	quint32 mFillMode;	// Fill mode
	quint32 mLastSpace;	// Last space

	Game *mGame;		// Game object
};

#endif // ENGINE_H
