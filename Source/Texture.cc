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

#include <QSize>
#include <QFile>
#include <QtDebug>

#include "Texture.h"



//----------------------------------------------------------------------------//
// Static                                                             Texture //
//----------------------------------------------------------------------------//

QList<Texture *> Texture::Textures;



//----------------------------------------------------------------------------//
// Constructors                                                       Texture //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>
/// <param name="pFilename"> Filename of the texture file. </param>
/// <param name="pName"> The name of the texture. </param>

Texture::Texture (QString pFilename, QString pName)
{
	Reset();
	mLoaded = Load (pFilename);
	Textures << this;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Destructor deallocating class resources. </summary>

Texture::~Texture (void)
{
	Textures.removeAt (Textures.indexOf (this));

	if (mData)
		delete mData;

	Reset();
}



//----------------------------------------------------------------------------//
// Draw                                                               Texture //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Texture::Draw (qint32 pX, qint32 pY)
{
	Draw (pX, pY, mWidth, mHeight);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Texture::Draw (qint32 pX, qint32 pY, quint32 pWidth, quint32 pHeight)
{
	if (!mLoaded)	return;

	glEnable		(GL_TEXTURE_2D);
	glBindTexture	(GL_TEXTURE_2D, mTexID);

	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode	(GL_MODELVIEW);
	glPushMatrix	();
	glLoadIdentity	();

	glTranslated	(pX, pY, 0);

	float lX = mMask.x();
	float lY = mMask.y();
	float lW = mMask.width();
	float lH = mMask.height();

	glBegin (GL_QUADS);
		glTexCoord2f (lX,    1-lY-lH); glVertex2i (0,           pHeight * lH);
		glTexCoord2f (lX+lW, 1-lY-lH); glVertex2i (pWidth * lW, pHeight * lH);
		glTexCoord2f (lX+lW, 1-lY   ); glVertex2i (pWidth * lW, 0           );
		glTexCoord2f (lX,    1-lY   ); glVertex2i (0,           0           );
	glEnd();

	glMatrixMode	(GL_MODELVIEW);
	glPopMatrix		();
}



//----------------------------------------------------------------------------//
// Accessors                                                          Texture //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the name of this texture. </summary>

QString Texture::Name (void) const
{
	return mName;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets the name of this image. </summary>

void Texture::SetName (QString pName)
{
	mName = pName;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QSize Texture::Size (void) const
{
	return QSize (mWidth, mHeight);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the width of this image. </summary>

quint32 Texture::Width (void) const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the height of this image. </summary>

quint32 Texture::Height (void) const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the pixel depth of this image. </summary>

quint32 Texture::Depth (void) const
{
	return mDepth;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns whether the texture is loaded. </summary>

bool Texture::Loaded (void) const
{
	return mLoaded;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

QRectF Texture::Mask (void) const
{
	return mMask;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Texture::SetMask (QRectF pMask)
{
	mMask = pMask;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the OpenGL Texture ID of this image. </summary>

quint32 Texture::TexID (void) const
{
	return mTexID;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the raw image data. </summary>

const quint8 *Texture::Data (void) const
{
	return mData;
}



//----------------------------------------------------------------------------//
// Static                                                             Texture //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Texture *Texture::FindTexture (QString pName)
{
	for (qint32 i = 0; i < Textures.size(); ++i)
		if (Textures.at(i)->Name() == pName)
			return Textures.at(i);

	return NULL;
}



//----------------------------------------------------------------------------//
// Internal                                                           Texture //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Texture::Reset (void)
{
	mName.clear();
	mData	= NULL;

	mWidth	= 0;
	mHeight	= 0;
	mDepth	= 0;

	mLoaded	= false;
	mTexID	= 0;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Loads an uncompressed TGA file. </summary>

bool Texture::Load (QString pFilename)
{
	//----------------------------------------------------------------------------//
	// Load                                                                       //
	//----------------------------------------------------------------------------//

	QFile lFile (pFilename);
	if (!lFile.open (QIODevice::ReadOnly))
	{
		qWarning() << "Unable to open file:" << pFilename;
		return false;
	}

	TgaHeader lHeader;
	ZeroMemory (&lHeader, sizeof (TgaHeader));

	if (lFile.read ((char *) &lHeader, sizeof (TgaHeader)) == -1)
	{
		qWarning() << "Unable to read texture header:" << pFilename;
		return false;
	}

	if (lHeader.ImageType != 2)
	{
		qWarning() << "Incorrect texture type:" << pFilename;
		return false;
	}



	//----------------------------------------------------------------------------//
	// Header                                                                     //
	//----------------------------------------------------------------------------//

	mWidth	= lHeader.ImageWidth;
	mHeight	= lHeader.ImageHeight;
	mDepth	= lHeader.PixelDepth;

	if (mWidth <= 0 || mHeight <= 0 ||
		(mDepth != 24 && mDepth != 32))
	{
		qWarning() << "Invalid texture dimension or depth:" << pFilename;
		return false;
	}

	// Skip Image ID
	lFile.seek (sizeof (TgaHeader) + lHeader.IdLength);



	//----------------------------------------------------------------------------//
	// Data                                                                       //
	//----------------------------------------------------------------------------//

	quint32 lDepth = mDepth / 8;
	quint32 lSize  = mWidth * mHeight * lDepth;

	mData = new quint8 [lSize];

	if (!lFile.read ((char *) mData, lSize))
	{
		qWarning() << "Failed to read texture data:" << pFilename;
		delete mData;
		mData = NULL;
		return false;
	}

	// Convert Tga BGR to RGB
	for (quint32 i = 0; i < lSize; i += lDepth)
	{
		quint8 temp = mData[i  ];
		mData[i  ]  = mData[i+2];
		mData[i+2]  = temp;
	}



	//----------------------------------------------------------------------------//
	// OpenGL                                                                     //
	//----------------------------------------------------------------------------//

	glGenTextures	(1, &mTexID);
	glBindTexture	(GL_TEXTURE_2D, mTexID);

	quint32 lType = (mDepth == 32) ? GL_RGBA : GL_RGB;

	glTexParameterf	(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf	(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D	(GL_TEXTURE_2D, 0, lType, mWidth, mHeight, 0, lType, GL_UNSIGNED_BYTE, mData);

	mMask = QRectF (0, 0, 1, 1);

	return true;
}
