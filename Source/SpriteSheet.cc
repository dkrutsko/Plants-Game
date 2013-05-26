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

#include "Texture.h"
#include "SpriteSheet.h"

#include <QSize>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QXmlStreamReader>



//----------------------------------------------------------------------------//
// Constructors                                                   SpriteSheet //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>

SpriteSheet::SpriteSheet (QString pFilename)
{
	Reset();
	mLoaded = Load (pFilename);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

SpriteSheet::~SpriteSheet (void)
{
	Reset();
}



//----------------------------------------------------------------------------//
// Draw                                                           SpriteSheet //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void SpriteSheet::Draw (QString pName, qint32 pX, qint32 pY, qreal pSx, qreal pSy) const
{
	if (!mLoaded || !mData.contains (pName))
		return;

	glEnable		(GL_TEXTURE_2D);
	glBindTexture	(GL_TEXTURE_2D, mTexture->TexID());

	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode	(GL_MODELVIEW);
	glPushMatrix	();
	glLoadIdentity	();

	glColor4f		(1.0f, 1.0f, 1.0f, 1.0f);

	glScalef		(pSx, pSy, 1);
	glTranslatef	(pX,  pY,  0);
	glCallList		(mData[pName].i);

	glMatrixMode	(GL_MODELVIEW);
	glPopMatrix		();
}



//----------------------------------------------------------------------------//
// Accessors                                                      SpriteSheet //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QString SpriteSheet::Name (void) const
{
	return mTexture->Name();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QSize SpriteSheet::Size (QString pName) const
{
	Sprite lSprite = mData[pName];
	return QSize (lSprite.w, lSprite.h);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 SpriteSheet::Width (QString pName) const
{
	return mData[pName].w;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

quint32 SpriteSheet::Height (QString pName) const
{
	return mData[pName].h;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool SpriteSheet::Loaded (void) const
{
	return mLoaded;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

const Texture *SpriteSheet::Tex (void) const
{
	return mTexture;
}



//----------------------------------------------------------------------------//
// Internal                                                       SpriteSheet //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void SpriteSheet::Reset (void)
{
	mList    = 0;
	mTexture = NULL;
	mLoaded  = false;

	mData.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool SpriteSheet::Load (QString pFilename)
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
	if (!lData.readNextStartElement() || lData.name() != "SpriteSheet")
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
		if (lToken == QXmlStreamReader::StartElement && lData.name() == "Sprite")
		{
			lAttr			= lData.attributes();

			QString lName	= lAttr.value ("Name").toString();
			quint32 lX		= lAttr.value ("X").toString().toUInt();
			quint32 lY		= lAttr.value ("Y").toString().toUInt();
			quint32 lWidth	= lAttr.value ("Width").toString().toUInt();
			quint32 lHeight	= lAttr.value ("Height").toString().toUInt();

			mData[lName]	= Sprite (lX, lY, lWidth, lHeight);
		}

	lFile.close();

	if (mData.size() == 0)
	{
		delete mTexture;
		mTexture = NULL;
		return true;
	}



	//----------------------------------------------------------------------------//
	// OpenGL                                                                     //
	//----------------------------------------------------------------------------//

	mList = glGenLists (mData.size());

	glBindTexture (GL_TEXTURE_2D, mTexture->TexID());

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	quint32 index = mList;
	QHashIterator<QString, Sprite> lIter (mData);

	while (lIter.hasNext())
	{
		lIter.next();

		Sprite lSprite		= lIter.value();
		lSprite.i			= index;
		mData[lIter.key()]	= lSprite;

		float lX = lSprite.x / (float) mTexture->Width();
		float lY = lSprite.y / (float) mTexture->Height();
		float lW = lSprite.w / (float) mTexture->Width();
		float lH = lSprite.h / (float) mTexture->Height();
		
		glNewList (index++, GL_COMPILE);

		glBegin (GL_QUADS);
			glTexCoord2f (lX,    1-lY-lH); glVertex2i (0,         lSprite.h);
			glTexCoord2f (lX+lW, 1-lY-lH); glVertex2i (lSprite.w, lSprite.h);
			glTexCoord2f (lX+lW, 1-lY   ); glVertex2i (lSprite.w, 0        );
			glTexCoord2f (lX,    1-lY   ); glVertex2i (0,         0        );
		glEnd();

		glEndList();
	}

	return true;
}
