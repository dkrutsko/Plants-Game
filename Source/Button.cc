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

#include <QList>

#include "Input.h"
#include "Button.h"



//----------------------------------------------------------------------------//
// Constructors                                                        Button //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Button::Button (const QRect &pGeometry, bool pSelectable)
	: Widget (pGeometry)
{
	mDown		= false;
	mPrevDown	= false;
	mSelected	= false;
	mSelectable	= pSelectable;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Button::Button (const QPoint &pPosition, const QSize &pSize, bool pSelectable)
	: Widget (pPosition, pSize)
{
	mDown		= false;
	mPrevDown	= false;
	mSelected	= false;
	mSelectable	= pSelectable;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Button::Button (quint32 pX, quint32 pY, quint32 pW, quint32 pH, bool pSelectable)
	: Widget (pX, pY, pW, pH)
{
	mDown		= false;
	mPrevDown	= false;
	mSelected	= false;
	mSelectable	= pSelectable;
}



//----------------------------------------------------------------------------//
// Accessors                                                           Button //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Button::Down (void) const
{
	return mDown;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Button::Selected (void) const
{
	return mSelected;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Button::SetSelectable (bool pSelectable)
{
	mSelectable = pSelectable;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Button::ToggleSelected (void)
{
	mSelected = !mSelected;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Button::SetSelected (bool pSelected)
{
	mSelected = pSelected;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QString Button::Type (void) const
{
	return QString ("Button");
}



//----------------------------------------------------------------------------//
// Update & Draw                                                       Button //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Button::Update (void)
{
	Widget::Update();

	mDown = mInside && Mouse::State()->Left();

	if (mSelectable && mInside && !mDown && mPrevDown)
		mSelected = !mSelected;

	mPrevDown = mDown;
}
