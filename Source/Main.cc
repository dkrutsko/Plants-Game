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
#include "Engine.h"

#include <cstdio>
#include <cstdlib>
#include <QCoreApplication>



//----------------------------------------------------------------------------//
// Message Handler                                                            //
//----------------------------------------------------------------------------//

void MessageOutput (QtMsgType type, const char *msg)
{
	switch (type)
	{
	case QtDebugMsg:
		fprintf (stderr, "Debug: %s\n", msg);
		break;

	case QtWarningMsg:
		fprintf (stderr, "Warning: %s\n", msg);
		break;

	case QtCriticalMsg:
		fprintf (stderr, "Critical: %s\n", msg);
		break;

	case QtFatalMsg:
		fprintf (stderr, "Fatal: %s\n", msg);
		exit(0);
	}
}



//----------------------------------------------------------------------------//
// Main                                                                       //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Main execution point for this application. </summary>
/// <param name="argc"> Number of arguments in the command line. </param>
/// <param name="argv"> Arguments from the command line. </param>
/// <returns> Zero for success, error code for failure. </returns>

int main (int argc, char **argv)
{
	qInstallMsgHandler (MessageOutput);
	QCoreApplication app (argc, argv);

	// Setup Application Info
	app.setApplicationVersion (QString ("1.1"));
	app.setApplicationName (QString ("Plants"));

	// Launch The Game
	return Engine().Run();
}
