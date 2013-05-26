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

#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Button : public Widget
{
public:
	// Constructors
	Button (const QRect &geometry, bool selectable = false);
	Button (const QPoint &position, const QSize &size, bool selectable = false);
	Button (quint32 x, quint32 y, quint32 w, quint32 h, bool selectable = false);

	// Accessors
	bool Down			(void) const;

	bool Selected		(void) const;
	void SetSelectable	(bool selectable);

	void ToggleSelected	(void);
	void SetSelected	(bool selected);

	virtual QString Type(void) const;

	// Update & Draw
	virtual void Update	(void);

protected:
	// Variables
	bool mDown;			// Button is down
	bool mPrevDown;		// Button is previously down

	bool mSelected;		// Button is selected
	bool mSelectable;	// Button is selectable
};

#endif // BUTTON_H
