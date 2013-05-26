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

#ifndef GAME_H
#define GAME_H

#include <QGlobal.h>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Button;
class Engine;
class ButtonGroup;

class Model;
class Entity;
class Emitter;
class Texture;
class SpriteSheet;
class EnemyManager;

////////////////////////////////////////////////////////////////////////////////
/// <summary> Controls the gameplay aspects of the game. </summary>

class Game
{
public:
	// Constructors
	 Game			(Engine *engine);
	~Game			(void);

	// Update & Draw
	void Update		(quint32 gametime);
	void Draw		(quint32 gametime);

	// Hacks
	static Entity *Selected	(void);
	static void Lost		(void);
	static void Won			(void);

private:
	// Internal
	void SelectPlant	(void);
	void PositionPlant	(void);
	void PickupSun		(void);

private:
	// Variables
	Engine *mEngine;

	float mSpeedX;
	float mSpeedY;
	float mEnvRotateX;
	float mEnvRotateY;

	Model *mEnv;
	Model *mField;

	Emitter *mSnow;
	Emitter *mClouds;

	static Entity *mSelected;

	SpriteSheet *mUi;
	Button *mButtons[4];
	ButtonGroup *mGroup;

	quint32 mLastSun;
	quint32 mNextSun;

	quint16 mSunlight;
	bool mLeftBtnDown;

	EnemyManager *mManager;

	Texture *mLogo;
	Texture *mTransition;
	Texture *mPressEnter;

	bool mDisplayPressEnter;
	quint32 mPressEnterTime;
	
	bool mMenuMode;
	qreal mMenuPos;

	Texture *mWonTex;
	Texture *mLostTex;

	static bool mLost;
	static bool mWon;
};

#endif // GAME_H
