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

#include "Button.h"
#include "ButtonGroup.h"



//----------------------------------------------------------------------------//
// Constructors                                                   ButtonGroup //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

ButtonGroup::ButtonGroup (void)
{
	mPrevSelected = -1;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

ButtonGroup::~ButtonGroup (void)
{
	mPrevSelected = -1;
	mButtons.clear();
}



//----------------------------------------------------------------------------//
// Update                                                         ButtonGroup //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void ButtonGroup::Update (void)
{
	for (qint32 i = 0; i < mButtons.size(); ++i)
		mButtons.at(i)->Update();

	if (mPrevSelected != -1 &&
		!mButtons.at (mPrevSelected)->Selected())
		mPrevSelected = -1;

	for (qint32 i = 0; i < mButtons.size(); ++i)
		if (mPrevSelected == -1 && mButtons.at(i)->Selected())
		{
			mPrevSelected = i;
			break;
		}

		else if (mPrevSelected != -1 &&
			mButtons.at(i)->Selected() && mPrevSelected != i)
			{
				mButtons.at(mPrevSelected)->ToggleSelected();
				mPrevSelected = i; break;
			}
}



//----------------------------------------------------------------------------//
// Methods                                                        ButtonGroup //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void ButtonGroup::AddButton (Button *pButton)
{
	mButtons << pButton;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void ButtonGroup::RemoveButton (Button *pButton)
{
	mButtons.removeAt (mButtons.indexOf (pButton));
}
