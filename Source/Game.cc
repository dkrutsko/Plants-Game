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
#include <SDL.h>

#define STRICT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include <QRect>
#include <QSize>
#include <QPoint>

#include "Game.h"
#include "Input.h"
#include "Button.h"
#include "Engine.h"
#include "ButtonGroup.h"

#include "Model.h"
#include "Entity.h"
#include "Random.h"
#include "Emitter.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "EnemyManager.h"

#include "Pea.h"
#include "Sun.h"
#include "Geode.h"
#include "Grunt.h"
#include "Walnut.h"
#include "Urchin.h"
#include "Sunflower.h"
#include "Cherrybomb.h"
#include "Peashooter.h"

#define PI 3.141592



//----------------------------------------------------------------------------//
// Static                                                                Game //
//----------------------------------------------------------------------------//

bool Game::mLost		= false;
bool Game::mWon			= false;
Entity *Game::mSelected	= NULL;



//----------------------------------------------------------------------------//
// Constructors                                                          Game //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>

Game::Game (Engine *pEngine)
{
	mEngine = pEngine;

	mSpeedX		= 0.0f;
	mSpeedY		= 0.0f;
	mEnvRotateX	=  45.0f;
	mEnvRotateY	= -45.0f;

	mSelected	= NULL;

	mLastSun	= 0;
	mNextSun	= Random (5000, 8000);

	mSunlight	= 50;
	mLeftBtnDown= false;

	mManager	= new EnemyManager();

	mMenuMode	= true;
	mMenuPos	= 0.0f;



	//----------------------------------------------------------------------------//
	// Models                                                                     //
	//----------------------------------------------------------------------------//

	Pea			::PreloadContent();
	Sun			::PreloadContent();
	Geode		::PreloadContent();
	Grunt		::PreloadContent();
	Walnut		::PreloadContent();
	Urchin		::PreloadContent();
	Sunflower	::PreloadContent();
	Cherrybomb	::PreloadContent();
	Peashooter	::PreloadContent();

	mField = new Model ("Content/Field.fbx");
	mEnv   = new Model ("Content/Environment.fbx");

	mSnow   = new Emitter ("Content/Snow.xml");
	mClouds = new Emitter ("Content/Clouds.xml");



	//----------------------------------------------------------------------------//
	// Interface                                                                  //
	//----------------------------------------------------------------------------//

	mLogo			= new Texture ("Content/Logo.tga");
	mTransition		= new Texture ("Content/Transition.tga");
	mPressEnter		= new Texture ("Content/PressEnter.tga");

	mWonTex			= new Texture ("Content/Won.tga");
	mLostTex		= new Texture ("Content/Lost.tga");

	mDisplayPressEnter	= false;
	mPressEnterTime		= 0;

	mGroup = new ButtonGroup();
	mUi    = new SpriteSheet ("Content/UI.xml");

	QSize lSize = mUi->Size ("Sunflower");
	for (int i = 0; i < 4; ++i)
	{
		mButtons[i] = new Button (QPoint (20,
			i * lSize.height() + 20), lSize, true);

		mGroup->AddButton (mButtons[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Destructor deallocating class resources. </summary>

Game::~Game (void)
{
	//----------------------------------------------------------------------------//
	// Models                                                                     //
	//----------------------------------------------------------------------------//

	SAFE_DELETE (mEnv)
	SAFE_DELETE (mField)

	while (Entity::Entities.size() > 0)
		delete Entity::Entities.takeAt(0);

	Pea			::UnloadContent();
	Sun			::UnloadContent();
	Geode		::UnloadContent();
	Grunt		::UnloadContent();
	Walnut		::UnloadContent();
	Urchin		::UnloadContent();
	Sunflower	::UnloadContent();
	Cherrybomb	::UnloadContent();
	Peashooter	::UnloadContent();

	SAFE_DELETE (mManager)
	SAFE_DELETE (mSnow)
	SAFE_DELETE (mClouds)



	//----------------------------------------------------------------------------//
	// Interface                                                                  //
	//----------------------------------------------------------------------------//

	SAFE_DELETE	(mLogo)
	SAFE_DELETE	(mTransition)
	SAFE_DELETE	(mPressEnter)

	SAFE_DELETE (mWonTex);
	SAFE_DELETE (mLostTex);

	SAFE_DELETE (mUi)
	SAFE_DELETE (mGroup)

	for (int i = 0; i < 4; ++i)
		SAFE_DELETE (mButtons[i])
}



//----------------------------------------------------------------------------//
// Update & Draw                                                         Game //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Updates the game. </summary>

void Game::Update (quint32 pGametime)
{
	//----------------------------------------------------------------------------//
	// Menu                                                                       //
	//----------------------------------------------------------------------------//

	if (mLost || mWon) return;

	Keyboard *lKeyboard = Keyboard::State();

	if (mMenuMode || mMenuPos < 1.0f)
	{
		if ((*lKeyboard)[SDLK_RETURN])
			mMenuMode = false;
		return;
	}



	//----------------------------------------------------------------------------//
	// Camera                                                                     //
	//----------------------------------------------------------------------------//

	if ((*lKeyboard)[SDLK_w] && !(*lKeyboard)[SDLK_s])
		if (mSpeedX < 1.4f && mEnvRotateX < 60.0f)
			mSpeedX += 0.16f;

	if ((*lKeyboard)[SDLK_s] && !(*lKeyboard)[SDLK_w])
		if (mSpeedX > -1.4f && mEnvRotateX > 20.0f)
			mSpeedX -= 0.16f;

		 if (mSpeedX > 0.10f)  mSpeedX -= 0.08f;
	else if (mSpeedX < -0.10f) mSpeedX += 0.08f;
	else mSpeedX = 0.0f;

	if ((*lKeyboard)[SDLK_d] && !(*lKeyboard)[SDLK_a])
		if (mSpeedY < 1.4f)
			mSpeedY += 0.16f;

	if ((*lKeyboard)[SDLK_a] && !(*lKeyboard)[SDLK_d])
		if (mSpeedY > -1.4f)
			mSpeedY -= 0.16f;

		 if (mSpeedY > 0.10f)  mSpeedY -= 0.08f;
	else if (mSpeedY < -0.10f) mSpeedY += 0.08f;
	else mSpeedY = 0.0f;

	mEnvRotateX += mSpeedX;
	mEnvRotateY += mSpeedY;

	Particle::RotateX = mEnvRotateX;
	Particle::RotateY = mEnvRotateY;



	//----------------------------------------------------------------------------//
	// Entities                                                                   //
	//----------------------------------------------------------------------------//

	for (int i = 0; i < Entity::Entities.size(); ++i)
		if (Entity::Entities.at(i) != mSelected)
			Entity::Entities.at(i)->Update (pGametime);

	mSnow->Update (pGametime);
	mClouds->Update (pGametime);



	//----------------------------------------------------------------------------//
	// Interface                                                                  //
	//----------------------------------------------------------------------------//

	mGroup->Update();

	if (mSelected == NULL)
		SelectPlant();

	if (mSelected != NULL)
		PositionPlant();



	//----------------------------------------------------------------------------//
	// Sunlight                                                                   //
	//----------------------------------------------------------------------------//

	if (mLastSun == 0)
		mLastSun = pGametime;

	if (pGametime - mLastSun > mNextSun &&
		mSunlight < 999 && Sun::SunCount() < 4)
	{
		Vector3 lPosition;
		lPosition.X = Random (-55, 55);
		lPosition.Z = Random (-25, 25);

		new Sun (lPosition);

		mLastSun = pGametime;
		mNextSun = Random (10000, 12000);
	}



	//----------------------------------------------------------------------------//
	// Enemies                                                                    //
	//----------------------------------------------------------------------------//

	mManager->Update (pGametime);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Draws the game. </summary>

void Game::Draw (quint32 pGametime)
{
	//----------------------------------------------------------------------------//
	// Game Won/Lost                                                              //
	//----------------------------------------------------------------------------//

	if (mLost || mWon)
	{
		SDL_ShowCursor (true);
		SDL_WM_GrabInput (SDL_GRAB_OFF);

		mEngine->SetMode2D();
		glDisable (GL_TEXTURE_2D);
		glColor3f (0, 0, 0);
		glBegin (GL_QUADS);
			glVertex2i (0,                mEngine->Height());
			glVertex2i (mEngine->Width(), mEngine->Height());
			glVertex2i (mEngine->Width(), 0                );
			glVertex2i (0,                0                );
		glEnd();
		glColor3f (1, 1, 1);

		if (mWon) mWonTex->Draw ((mEngine->Width()  / 2) - (mWonTex->Width()  / 2),
								 (mEngine->Height() / 2) - (mWonTex->Height() / 2));

		else mLostTex->Draw ((mEngine->Width()  / 2) - (mLostTex->Width()  / 2),
							 (mEngine->Height() / 2) - (mLostTex->Height() / 2));

		return;
	}



	//----------------------------------------------------------------------------//
	// Models                                                                     //
	//----------------------------------------------------------------------------//

	mEngine->SetMode3D();
	glTranslatef (0, 0, -175.0f);
	glRotatef (mEnvRotateX, 1, 0, 0);
	glRotatef (mEnvRotateY, 0, 1, 0);

	mEnv->Draw   (pGametime);
	mField->Draw (pGametime);

	for (int i = 0; i < Entity::Entities.size(); ++i)
		Entity::Entities.at (i)->Draw (pGametime);

	PickupSun();

	mSnow->Draw (pGametime);
	mClouds->Draw (pGametime);



	//----------------------------------------------------------------------------//
	// Interface                                                                  //
	//----------------------------------------------------------------------------//

	mEngine->SetMode2D();
	QSize lSize = mUi->Size ("Sunflower");

	// Buttons
	if (mButtons[0]->Selected() || (mButtons[0]->Inside() && !mSelected))
		 mUi->Draw ("SSunflower", 20, 20);
	else mUi->Draw ("Sunflower", 20, 20);

	if (mButtons[1]->Selected() || (mButtons[1]->Inside() && !mSelected))
		 mUi->Draw ("SPeashooter", 20, lSize.height() + 20);
	else mUi->Draw ("Peashooter", 20, lSize.height() + 20);

	if (mButtons[2]->Selected() || (mButtons[2]->Inside() && !mSelected))
		 mUi->Draw ("SWalnut", 20, lSize.height() * 2 + 20);
	else mUi->Draw ("Walnut", 20, lSize.height() * 2 + 20);

	if (mButtons[3]->Selected() || (mButtons[3]->Inside() && !mSelected))
		 mUi->Draw ("SCherrybomb", 20, lSize.height() * 3 + 20);
	else mUi->Draw ("Cherrybomb", 20, lSize.height() * 3 + 20);

	// Sunlight and Progress
	mUi->Draw ("Score", mEngine->Width()  - mUi->Width  ("Score") - 20,
						mEngine->Height() - mUi->Height ("Score") - 20);

//	mUi->Draw ("Progress", mEngine->Width()  - mUi->Width  ("Progress") - mUi->Width  ("Score"),
//						   mEngine->Height() - mUi->Height ("Progress") - 20);

	// Sunlight Amount
	QString lSunlight = QString::number (mSunlight);
	for (qint32 i = 0; i < lSunlight.size(); ++i)
		mUi->Draw (lSunlight.at (lSunlight.size() - i - 1),
			mEngine->Width() - 95 - (i * 65), mEngine->Height() - 120);



	//----------------------------------------------------------------------------//
	// Menu                                                                       //
	//----------------------------------------------------------------------------//

	mEngine->SetMode2D();
	if (mMenuMode || mMenuPos < 1.0f)
	{
		if (!mMenuMode)
			mMenuPos += 0.02f;

		qint32 lLogoPos = (mEngine->Width()  / 2.0f) - (mLogo->Width()  / 2.0f);
		qint32 lPePos = (mEngine->Width() / 2.0f) - (mPressEnter->Width() / 2.0f);

		qint32 lXPos = (mMenuPos * mEngine->Width()) -
			(mTransition->Width() * (1 - mMenuPos));
		float lHalf = mTransition->Width() / 2.0f - 12.0f;

		QRectF lLogoMask = QRect (0, 0, 1, 1);
		if (lXPos + lHalf > lLogoPos && lXPos + lHalf < lLogoPos + mLogo->Width())
		{
			lLogoMask.setX ((lXPos + lHalf - lLogoPos) / mLogo->Width());
			lLogoPos += lXPos + lHalf - lLogoPos;
			mLogo->SetMask (lLogoMask);
		}

		glDisable (GL_TEXTURE_2D);
		glColor3f (0, 0, 0);
		glBegin (GL_QUADS);
			glVertex2i (lXPos + lHalf,    mEngine->Height());
			glVertex2i (mEngine->Width(), mEngine->Height());
			glVertex2i (mEngine->Width(), 0                );
			glVertex2i (lXPos + lHalf,    0                );
		glEnd();
		glColor3f (1, 1, 1);

		mLogo->Draw (lLogoPos, (mEngine->Height() / 2.0f) - (mLogo->Height() / 2.0f));

		if (mMenuMode && mDisplayPressEnter)
			mPressEnter->Draw ((mEngine->Width() / 2) - (mPressEnter->Width() / 2),
								mEngine->Height() - 40 - mPressEnter->Height());

		if (pGametime - mPressEnterTime > 1000)
		{
			mPressEnterTime		= pGametime;
			mDisplayPressEnter	= !mDisplayPressEnter;
		}

		mTransition->Draw (lXPos, 0.0f, mTransition->Width(), mEngine->Height());
	}
}



//----------------------------------------------------------------------------//
// Internal                                                              Game //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Selects an entity based on the option. </summary>

void Game::SelectPlant (void)
{
	if (mSelected != NULL)
	{
		delete mSelected;
		mSelected = NULL;
	}

	for (quint32 i = 0; i < 4; ++i)
		if (mButtons[i]->Selected())
		{
			if (i == 0 && mSunlight >= 50)
				mSelected = new Sunflower();

			else if (i == 1 && mSunlight >= 100)
				mSelected = new Peashooter();

			else if (i == 2 && mSunlight >= 50)
				mSelected = new Walnut();

			else if (i == 3 && mSunlight >= 150)
				mSelected = new Cherrybomb();

			mButtons[i]->SetSelected (false);
			break;
		}

	if (mSelected != NULL)
	{
		SDL_ShowCursor (false);
		SDL_WM_GrabInput (SDL_GRAB_ON);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Positions the currently selected entity. </summary>

void Game::PositionPlant (void)
{
	Mouse *lMouse = Mouse::State();
	SDL_WarpMouse (mEngine->Width() / 2, mEngine->Height() / 2);

	float lChangeX = (mEngine->Width()  / 2.0f - lMouse->X()) / 40.0f;
	float lChangeY = (mEngine->Height() / 2.0f - lMouse->Y()) / 40.0f;

	// Line up based on rotation
	float lRadians = mEnvRotateY * (PI / 180);
	mSelected->Position.X -= lChangeX * cos (lRadians);
	mSelected->Position.Z -= lChangeX * sin (lRadians);
	mSelected->Position.X -= lChangeY * sin (-lRadians);
	mSelected->Position.Z -= lChangeY * cos (-lRadians);

	if (mSelected->Position.X < -55.0f) mSelected->Position.X = -55.0f;
	if (mSelected->Position.X >  55.0f) mSelected->Position.X =  55.0f;
	if (mSelected->Position.Z < -25.0f) mSelected->Position.Z = -25.0f;
	if (mSelected->Position.Z >  25.0f) mSelected->Position.Z =  25.0f;

	if (lMouse->Left())
	{
		// Snap to the grid
		mSelected->Position.X = floor (mSelected->Position.X / 13 + 0.5f) * 13;
		mSelected->Position.Z = floor (mSelected->Position.Z / 12 + 0.5f) * 12;

		for (int i = 0; i < Entity::Entities.size(); ++i)
			if (Entity::Entities.at(i) != mSelected &&
				Entity::Entities.at(i)->Type() != Entity::eSun &&
				abs (Entity::Entities.at(i)->Position.X - mSelected->Position.X) < 2 &&
				abs (Entity::Entities.at(i)->Position.Z - mSelected->Position.Z) < 2)
				return;

		SDL_ShowCursor (true);
		SDL_WM_GrabInput (SDL_GRAB_OFF);
		for (quint32 i = 0; i < 4; ++i)
			mButtons[i]->SetSelected (false);

		if (mSelected->Type() == Entity::eSunflower)
			mSunlight -= 50;
		else if (mSelected->Type() == Entity::ePeashooter)
			mSunlight -= 100;
		else if (mSelected->Type() == Entity::eWalnut)
			mSunlight -= 50;
		else if (mSelected->Type() == Entity::eCherrybomb)
			mSunlight -= 150;

		mSelected = NULL;
	}

	else if (lMouse->Right())
	{
		SDL_ShowCursor (true);
		SDL_WM_GrabInput (SDL_GRAB_OFF);
		for (quint32 i = 0; i < 4; ++i)
			mButtons[i]->SetSelected (false);

		delete mSelected;
		mSelected = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Handle selection of sunlight. </summary>

void Game::PickupSun (void)
{
	Mouse *lMouse = Mouse::State();

	if (!lMouse->Left())
		mLeftBtnDown = false;

	if (!lMouse->Left() || mLeftBtnDown)
		return;

	mLeftBtnDown = true;

	qreal lX1, lY1, lZ1;
	qreal lX2, lY2, lZ2;

	qint32 lViewport   [ 4];
	qreal  lModelView  [16];
	qreal  lProjection [16];

	glGetIntegerv (GL_VIEWPORT,			 lViewport);
	glGetDoublev  (GL_MODELVIEW_MATRIX,  lModelView);
	glGetDoublev  (GL_PROJECTION_MATRIX, lProjection);

	gluUnProject (lMouse->X(), lViewport[3] - lMouse->Y(), 0.0f,
		lModelView, lProjection, lViewport, &lX1, &lY1, &lZ1);
	gluUnProject (lMouse->X(), lViewport[3] - lMouse->Y(), 1.0f,
		lModelView, lProjection, lViewport, &lX2, &lY2, &lZ2);

	Vector3 lNearPoint (lX1, lY1, lZ1);
	Vector3 lFarPoint  (lX2, lY2, lZ2);
	Vector3 lRayVector (lX2 - lX1, lY2 - lY1, lZ2 - lZ1);

	float lRayLength = lRayVector.Length();

	lRayVector.X /= lRayLength;
	lRayVector.Y /= lRayLength;
	lRayVector.Z /= lRayLength;

	Sun *lSelected = NULL;
	Vector3 lCollisionPoint;

	for (qint32 i = 0; i < Entity::Entities.size(); ++i)
		if (Entity::Entities.at(i)->Type() == Entity::eSun)
		{
			Vector3 lPosition = Entity::Entities.at(i)->Position;

			for (quint32 j = 0; j < 200; ++j)
			{
				lCollisionPoint = lNearPoint + (lRayVector * (j * 3.0f));
				lCollisionPoint.Y -= 3.0f;

				if (pow (lCollisionPoint.X - lPosition.X, 2) +
					pow (lCollisionPoint.Y - lPosition.Y, 2) +
					pow (lCollisionPoint.Z - lPosition.Z, 2) < pow (3.0f, 2))
				{
					lSelected = (Sun *) Entity::Entities.at(i);
					break;
				}

				else if (lCollisionPoint.Y < -10)
					break;
			}

			if (lSelected != NULL)
				break;
		}

	if (lSelected != NULL &&
		!lSelected->Selected &&
		!lSelected->Falling)
	{
		mSunlight += 25;
		lSelected->Selected = true;

		if (mSunlight > 999)
			mSunlight = 999;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Entity *Game::Selected (void)
{
	return mSelected;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Game::Lost (void)
{
	mLost = true;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Game::Won (void)
{
	mWon = true;
}
