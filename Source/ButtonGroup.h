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

#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include <QList>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Button;

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class ButtonGroup
{
	friend class Button;

public:
	// Constructors
	 ButtonGroup		(void);
	~ButtonGroup		(void);

	// Update
	void Update			(void);

	// Methods
	void AddButton		(Button *button);
	void RemoveButton	(Button *button);

private:
	// Variables
	qint32 mPrevSelected;
	QList<Button *> mButtons;
};

#endif // BUTTON_GROUP_H
