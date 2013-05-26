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

#include "Random.h"
#include "Vector3.h"
#include "EnemyManager.h"

#include "Geode.h"
#include "Grunt.h"
#include "Urchin.h"



//----------------------------------------------------------------------------//
// Constructors                                                  EnemyManager //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

EnemyManager::EnemyManager (void)
{
	mWave      = 0;
	mLastEnemy = 0;
	mNextEnemy = 0;

	mFinished  = false;
}



//----------------------------------------------------------------------------//
// Methods                                                       EnemyManager //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void EnemyManager::Update (quint32 pGametime)
{
	if (mNextEnemy == 0)
	{
		mLastEnemy = pGametime;
		mNextEnemy = Random (26000, 28000);
	}

	if (!mFinished && pGametime - mLastEnemy > mNextEnemy)
	{
		++mWave;
		mLastEnemy = pGametime;

		mNextEnemy = Random (20000, 22000);

		if (mWave == 3)
			mNextEnemy = Random (1000, 1500);

		if (mWave == 6 || mWave == 7)
			mNextEnemy = Random (1000, 1500);

		if (mWave == 10)
			mNextEnemy = Random (1000, 1500);

		if (mWave >= 12 && mWave <= 18)
			mNextEnemy = Random (1000, 1500);

		if (mWave == 19)
			mNextEnemy = 30000;

		if (mWave != 20)
			 Create (false);
		else
		{
			Create (true);
			mFinished = true;
		}
	}
}



//----------------------------------------------------------------------------//
// Internal                                                      EnemyManager //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void EnemyManager::Create (bool pBoss)
{
	static float lStart[] = { -24, -12, 0, 12, 24 };

	Vector3 lPosition;
	lPosition.X = 60;
	lPosition.Z = lStart [Random (100) % 5];

	if (pBoss)
		 new Grunt (lPosition);
	else
		if (mWave == 1 || mWave == 2)
			new Geode (lPosition);
		
		else if (Random (0, 2) == 0)
			 new Geode (lPosition);
		else new Urchin (lPosition);
}
