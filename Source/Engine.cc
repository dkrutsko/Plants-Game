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

#include <SDL.h>

#define STRICT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include <QSize>

#include "Game.h"
#include "Input.h"
#include "Engine.h"
#include "Random.h"
#include "Vector3.h"
#include "Texture.h"

#include <QByteArray>
#include <QCoreApplication>



//----------------------------------------------------------------------------//
// Engine                                                              Engine //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the game. </summary>

int Engine::Run (void)
{
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		qWarning ("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (SDL_SetVideoMode (WIDTH, HEIGHT, 0, SDL_OPENGL | SDL_RESIZABLE) == NULL)
	{
		qWarning ("Unable to create OpenGL scene: %s", SDL_GetError());
		return 2;
	}

	// Enable Anti Aliasing x8
	SDL_GL_SetAttribute (SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute (SDL_GL_MULTISAMPLESAMPLES, 8);

	QByteArray lData = QCoreApplication::applicationName().toAscii();
	const char *lAppName = lData.constData();

	SDL_WM_SetCaption (lAppName, NULL);

	mWidth	= WIDTH;
	mHeight	= HEIGHT;

	mFillMode  = Engine::Solid;
	mLastSpace = 0;

	// Initialize the game
	Initialize();
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetMode2D();

	Texture *lLoading;
	if (Random (0, 2) == 0)
		 lLoading = new Texture ("Content/Loading1.tga", "Loading");
	else lLoading = new Texture ("Content/Loading2.tga", "Loading");
	lLoading->Draw ((mWidth  / 2.0f) - (lLoading->Width()  / 2.0f),
					(mHeight / 2.0f) - (lLoading->Height() / 2.0f));
	glFlush(); SDL_GL_SwapBuffers();

	mExit = false;
	mGame = new Game (this);

	SDL_Event lEvent;

	const  long lUpdateTime	= (long) (1000 / 60);
	static long lLastTime	= SDL_GetTicks();

	Keyboard *lKeyboard = Keyboard::State();
	Mouse    *lMouse    = Mouse   ::State();

	// Enter the game loop
	while (!mExit)
	{
		// Update the game
		long lNewTime = SDL_GetTicks();
		if (lNewTime - lLastTime > lUpdateTime)
		{
			mGame->Update (lNewTime);
			lLastTime = lNewTime;
		}

		// Draw the game
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mGame->Draw (lNewTime);

		glFlush();
		SDL_GL_SwapBuffers();

		// Update SDL events
		while (SDL_PollEvent (&lEvent))
			switch (lEvent.type)
			{
				case SDL_QUIT:
					mExit = true;
					break;

				case SDL_VIDEORESIZE: 
					mWidth  = lEvent.resize.w;
					mHeight = lEvent.resize.h;
					break;
			}

		if ((*lKeyboard)[SDLK_ESCAPE])
			mExit = true;

		if ((*lKeyboard)[SDLK_SPACE] && (lNewTime - mLastSpace) > 250)
		{
			mFillMode = ++mFillMode % 3;
			mLastSpace = lNewTime;
		}
	}

	// Finish the game
	delete mGame;

	Uninitialize();
	SDL_Quit();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets the OpenGL draw mode to be 2D. </summary>

void Engine::SetMode2D (void)
{
	glViewport (0, 0, mWidth, mHeight);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D (0, mWidth, mHeight, 0);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets the OpenGL draw mode to be 3D. </summary>

void Engine::SetMode3D (void)
{
	glViewport (0, 0, mWidth, mHeight);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluPerspective (45.0f, (float) mWidth / (float) mHeight, 0.1f, 10000.0f);

	switch (mFillMode)
	{
		case Engine::Solid	: glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);  break;
		case Engine::Line	: glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);  break;
		case Engine::Point	: glPolygonMode (GL_FRONT_AND_BACK, GL_POINT); break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Instructs the game to exit next tick. </summary>

void Engine::Exit (void)
{
	mExit = true;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the window width of the game. </summary>

int Engine::Width (void) const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the window height of the game. </summary>

int Engine::Height (void) const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QSize Engine::Size (void) const
{
	return QSize (mWidth, mHeight);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Engine::FillMode Engine::GetFillMode (void) const
{
	return (Engine::FillMode) mFillMode;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Engine::SetFillMode (Engine::FillMode pMode)
{
	mFillMode = pMode;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Makes the game better. </summary>

void Engine::MakeBetter (void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Makes you Sweet French Vanilla Latte. </summary>

void Engine::MakeCoffee (void)
{
}



//----------------------------------------------------------------------------//
// Internal                                                            Engine //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes game engine components. </summary>

void Engine::Initialize (void)
{
	glShadeModel	(GL_SMOOTH);
	glClearColor	(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable		(GL_DEPTH_TEST);
	glDepthFunc		(GL_LEQUAL);

	glHint			(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable		(GL_PERSPECTIVE_CORRECTION_HINT);

	glPointSize		(2.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Uninitializes game engine components. </summary>

void Engine::Uninitialize (void)
{
	// Delete all textures
	while (Texture::Textures.size() > 0)
		delete Texture::Textures.takeAt (0);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Updates the FPS in the window's caption. </summary>

void Engine::UpdateFPS (void)
{
	static long lLastTime	= SDL_GetTicks();
	static long lLoops		= 0;
	static float lFPS		= 0.0f;

	int lNewTime = SDL_GetTicks();
	if (lNewTime - lLastTime > 60)
	{
		float lNewFPS = (float) lLoops / float (lNewTime - lLastTime) * 1000.0f;

		lFPS = (lFPS + lNewFPS) / 2.0f;

		char title[80];
		sprintf_s (title, 80, "Plants Vs Protoss - %.2f", lFPS);

		SDL_WM_SetCaption (title, NULL);
		lLastTime = lNewTime; lLoops = 0;
	}

	++lLoops;
}
