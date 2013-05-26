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

#define STRICT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Random.h"
#include "Emitter.h"
#include "Texture.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QXmlStreamReader>



//----------------------------------------------------------------------------//
// Constructors                                                       Emitter //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Emitter::Emitter (QString pFilename)
{
	Reset();
	mLoaded = Load (pFilename);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Emitter::~Emitter (void)
{
	while (mParticles.size() > 0)
		delete mParticles.takeAt (0);

	Reset();
}



//----------------------------------------------------------------------------//
// Update & Draw                                                      Emitter //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Emitter::Update (quint32 pGametime)
{
	if (mTexture == NULL) return;
	if (mLastTime == 0)
		mLastTime = pGametime;

	quint32 lNumEmission = (quint32) ((float)
		(pGametime - mLastTime) / 1000.0f * Rate);

	for (quint32 i = 0; i < lNumEmission; ++i)
		AddParticle();

	if (lNumEmission > 0)
		mLastTime = pGametime;

	for (qint32 i = 0; i < mParticles.size(); ++i)
	{
		mParticles.at(i)->Update (pGametime);
		if (!mParticles.at(i)->Active())
		{
			delete mParticles.at(i);
			mParticles.removeAt (i);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Emitter::Draw (quint32 pGametime)
{
	if (mTexture == NULL) return;

	glEnable		(GL_TEXTURE_2D);
	glBindTexture	(GL_TEXTURE_2D, mTexture->TexID());

	glDepthMask		(GL_FALSE);
	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode	(GL_MODELVIEW);
	glPushMatrix	();
	glLoadIdentity	();

	glTranslatef	(Position.X, Position.Y, Position.Z);

	for (qint32 i = 0; i < mParticles.size(); ++i)
		mParticles.at(i)->Draw (pGametime);

	glMatrixMode	(GL_MODELVIEW);
	glPopMatrix		();

	glDepthMask		(GL_TRUE);
}



//----------------------------------------------------------------------------//
// Accessors                                                          Emitter //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

const Texture *Emitter::Tex (void) const
{
	return mTexture;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QString Emitter::Name (void) const
{
	return mName;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Emitter::Loaded (void) const
{
	return mLoaded;
}



//----------------------------------------------------------------------------//
// Internal                                                           Emitter //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Emitter::Reset (void)
{
	mParticles.clear();

	mTexture	= NULL;
	mLastTime	= 0;

	Rate		= 30.0f;
	Radius		= Vector3();

	LifeMin		= 1.5f;
	LifeMax		= 2.5f;

	SizeMin		= 3.0f;
	SizeMax		= 6.0f;

	Saturation	= 0.0f;
	Color		= Vector3 (1, 1, 1);
	Alpha		= 0.5f;

	Spread		= Vector3 (1, 0, 1);
	Gravity		= 0.0f;

	Position	= Vector3();
	Rotation	= Vector3();
	Wind		= Vector3();

	Bounciness	= 0.9f;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Emitter::AddParticle (void)
{
	Particle *lParticle = new Particle ((int) mParticles.size());

//	float r = Random (0.0f, 1.0f) * Saturation + (1 - Saturation);
//	float g = Random (0.0f, 1.0f) * Saturation + (1 - Saturation);
//	float b = Random (0.0f, 1.0f) * Saturation + (1 - Saturation);

	float vx = Random (-Spread.X, Spread.X);
	float vy = Random (-Spread.Y, Spread.Y);
	float vz = Random (-Spread.Z, Spread.Z);

	lParticle->Position.X	= Random (-Radius.X, Radius.X);
	lParticle->Position.Y	= Random (-Radius.Y, Radius.Y);
	lParticle->Position.Z	= Random (-Radius.Z, Radius.Z);

	lParticle->Rotation		= Rotation;
	lParticle->Velocity		= Vector3 (vx, vy, vz);

	lParticle->Acceleration.Y	= -Gravity;
	lParticle->Acceleration		+= Wind;

//	lParticle->Color		= Vector3 (r, g, b);
	lParticle->Color		= Color;

	lParticle->TotalLife	= Random (LifeMin, LifeMax);
	lParticle->Life			= lParticle->TotalLife;

	lParticle->Alpha		= Alpha;
	lParticle->Size			= Random (SizeMin, SizeMax);

	lParticle->Ground		= Position.Y;
	lParticle->Bounciness	= Bounciness;

	mParticles << lParticle;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Emitter::Load (QString pFilename)
{
	//----------------------------------------------------------------------------//
	// Load                                                                       //
	//----------------------------------------------------------------------------//
	
	// Load file
	QFile lFile (pFilename);
	if (!lFile.open (QFile::ReadOnly | QFile::Text))
	{
		qWarning() << "Unable to open file:" << pFilename;
		return false;
	}

	QXmlStreamReader lData;
	lData.setDevice (&lFile);

	// Validate tag name
	if (!lData.readNextStartElement() || lData.name() != "Emitter")
	{
		qWarning() << "Invalid SpriteSheet format:" << pFilename;
		return false;
	}

	// Validate version
	QXmlStreamAttributes lAttr = lData.attributes();
	if (lAttr.value ("Version").toString() != "2011.4")
	{
		qWarning() << "Invalid SpriteSheet version:" << pFilename;
		return false;
	}

	// Load Texture and SpriteSheet name
	QString lName = lAttr.value ("Name").toString();
	QString lFileName = QFileInfo (lFile).path() +
		"/" + lAttr.value ("Texture").toString();

	if (QFileInfo (lFileName).suffix().toUpper() != "TGA")
	{
		qWarning() << "Only TGA textures are supported:" << pFilename;
		return false;
	}

	Texture *lTexture = Texture::FindTexture (lFileName);
	if (mTexture == NULL)
		mTexture = new Texture (lFileName, lFileName);

	if (!mTexture->Loaded())
	{
		qWarning() << "Could not load texture:" << pFilename;
		delete mTexture;
		mTexture = NULL;
		return false;
	}



	//----------------------------------------------------------------------------//
	// Data                                                                       //
	//----------------------------------------------------------------------------//
	
	QXmlStreamReader::TokenType lToken;

	while ((lToken = lData.readNext()) != QXmlStreamReader::Invalid)
		if (lToken == QXmlStreamReader::StartElement)
		{
			lAttr = lData.attributes();
			if (lData.name() == "Rate")
				Rate = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "Radius")
			{
				Radius.X = lAttr.value ("X").toString().toFloat();
				Radius.Y = lAttr.value ("Y").toString().toFloat();
				Radius.Z = lAttr.value ("Z").toString().toFloat();
			}

			else if (lData.name() == "LifeMin")
				LifeMin = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "LifeMax")
				LifeMax = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "SizeMin")
				SizeMin = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "SizeMax")
				SizeMax = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "Saturation")
				Saturation = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "Color")
			{
				Color.X = lAttr.value ("R").toString().toUInt() % 256;
				Color.Y = lAttr.value ("G").toString().toUInt() % 256;
				Color.Z = lAttr.value ("B").toString().toUInt() % 256;
			}

			else if (lData.name() == "Alpha")
				Alpha = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "Spread")
			{
				Spread.X = lAttr.value ("X").toString().toFloat();
				Spread.Y = lAttr.value ("Y").toString().toFloat();
				Spread.Z = lAttr.value ("Z").toString().toFloat();
			}

			else if (lData.name() == "Gravity")
				Gravity = lAttr.value ("Value").toString().toFloat();

			else if (lData.name() == "Position")
			{
				Position.X = lAttr.value ("X").toString().toFloat();
				Position.Y = lAttr.value ("Y").toString().toFloat();
				Position.Z = lAttr.value ("Z").toString().toFloat();
			}

			else if (lData.name() == "Rotation")
			{
				Rotation.X = lAttr.value ("X").toString().toFloat();
				Rotation.Y = lAttr.value ("Y").toString().toFloat();
				Rotation.Z = lAttr.value ("Z").toString().toFloat();
			}

			else if (lData.name() == "Wind")
			{
				Wind.X = lAttr.value ("X").toString().toFloat();
				Wind.Y = lAttr.value ("Y").toString().toFloat();
				Wind.Z = lAttr.value ("Z").toString().toFloat();
			}

			else if (lData.name() == "Bounciness")
				Bounciness = lAttr.value ("Value").toString().toFloat();
		}

	lFile.close();
	return true;
}
