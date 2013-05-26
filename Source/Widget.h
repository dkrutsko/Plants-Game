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

#ifndef WIDGET_H
#define WIDGET_H

#include <QString>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class QRect;
class QSize;
class QPoint;

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Widget
{
public:
	// Constructors
	Widget (void);
	Widget (const QRect &geometry);
	Widget (const QPoint &position, const QSize &size);
	Widget (quint32 x, quint32 y, quint32 w, quint32 h);

	virtual ~Widget (void) { }

	// Accessors
	bool Inside			(void) const;

	quint32 X			(void) const;
	quint32 Y			(void) const;
	quint32 Width		(void) const;
	quint32 Height		(void) const;

	QSize   Size		(void) const;
	QPoint  Position	(void) const;
	QRect   Geometry	(void) const;

	void SetX			(quint32 x);
	void SetY			(quint32 y);
	void SetWidth		(quint32 w);
	void SetHeight		(quint32 h);

	void SetSize		(const QSize &size);
	void SetSize		(quint32 w, quint32 h);

	void SetPosition	(const QPoint &position);
	void SetPosition	(quint32 x, quint32 y);

	void SetGeometry	(const QRect &geometry);
	void SetGeometry	(quint32 x, quint32 y, quint32 w, quint32 h);

	virtual QString Type(void) const = 0;

	// Update & Draw
	virtual void Update	(void);
	virtual void Draw	(void) const { }

protected:
	// Variables
	bool mInside;	// Mouse is inside this Widget

	quint32 mX;		// X position
	quint32 mY;		// Y position
	quint32 mW;		// Width
	quint32 mH;		// Height
};

#endif // WIDGET_H
