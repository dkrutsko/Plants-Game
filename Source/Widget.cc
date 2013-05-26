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

#include "Input.h"
#include "Widget.h"

#include <QRect>
#include <QSize>
#include <QPoint>



//----------------------------------------------------------------------------//
// Constructors                                                        Widget //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Widget::Widget (void)
{
	mX = 0;
	mY = 0;
	mW = 0;
	mH = 0;

	mInside = false;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Widget::Widget (const QRect &pGeometry)
{
	mX = pGeometry.x();
	mY = pGeometry.y();
	mW = pGeometry.width();
	mH = pGeometry.height();

	mInside = false;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Widget::Widget (const QPoint &pPosition, const QSize &pSize)
{
	mX = pPosition.x();
	mY = pPosition.y();
	mW = pSize.width();
	mH = pSize.height();

	mInside = false;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Widget::Widget (quint32 pX, quint32 pY, quint32 pW, quint32 pH)
{
	mX = pX;
	mY = pY;
	mW = pW;
	mH = pH;

	mInside = false;
}



//----------------------------------------------------------------------------//
// Accessors                                                           Widget //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Widget::Inside (void) const
{
	return mInside;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 Widget::X (void) const
{
	return mX;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 Widget::Y (void) const
{
	return mY;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 Widget::Width (void) const
{
	return mW;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 Widget::Height (void) const
{
	return mH;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QSize Widget::Size (void) const
{
	return QSize (mW, mH);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QPoint Widget::Position (void) const
{
	return QPoint (mX, mY);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QRect Widget::Geometry (void) const
{
	return QRect (mX, mY, mW, mH);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetX (quint32 pX)
{
	mX = pX;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetY (quint32 pY)
{
	mY = pY;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetWidth (quint32 pW)
{
	mW = pW;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetHeight (quint32 pH)
{
	mH = pH;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetSize (const QSize &pSize)
{
	mW = pSize.width();
	mH = pSize.height();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetSize (quint32 pW, quint32 pH)
{
	mW = pW;
	mH = pH;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetPosition (const QPoint &pPosition)
{
	mX = pPosition.x();
	mY = pPosition.y();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetPosition (quint32 pX, quint32 pY)
{
	mX = pX;
	mY = pY;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetGeometry (const QRect &pGeometry)
{
	mX = pGeometry.x();
	mY = pGeometry.y();
	mW = pGeometry.width();
	mH = pGeometry.height();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::SetGeometry (quint32 pX, quint32 pY, quint32 pW, quint32 pH)
{
	mX = pX;
	mY = pY;
	mW = pW;
	mH = pH;
}



//----------------------------------------------------------------------------//
// Update & Draw                                                       Widget //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Widget::Update (void)
{
	Mouse *lMouse = Mouse::State();

	mInside = false;
	if (lMouse->X() >= mX && lMouse->X() <= mX + mW &&
		lMouse->Y() >= mY && lMouse->Y() <= mY + mH)
		mInside = true;
}
