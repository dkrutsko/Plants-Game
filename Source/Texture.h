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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <QRect>
#include <QList>
#include <QString>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class QSize;

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a simple texture. </summary>
/// <remarks> Can load an uncompressed TGA file. </remarks>

class Texture
{
private:
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Represents the header of a TGA file. </summary>

	class TgaHeader
	{
	public:
		// Variables
		quint8 IdLength;
		quint8 ColorMapType;
		quint8 ImageType;
		quint8 ColorMapSpecification[5];

		qint16 OriginX;
		qint16 OriginY;
		qint16 ImageWidth;
		qint16 ImageHeight;

		quint8 PixelDepth;
		quint8 ImageDescriptor;
	};

public:
	// Constructors
	 Texture			(QString filename, QString name = QString());
	~Texture			(void);

	// Draw
	void Draw			(qint32 x, qint32 y);
	void Draw			(qint32 x, qint32 y, quint32 width, quint32 height);

	// Accessors
	QString Name		(void) const;
	void SetName		(QString name);

	QSize Size			(void) const;
	quint32 Width		(void) const;
	quint32 Height		(void) const;
	quint32 Depth		(void) const;

	bool Loaded			(void) const;

	QRectF Mask			(void) const;
	void SetMask		(QRectF mask);

	quint32 TexID		(void) const;
	const quint8 *Data	(void) const;

	// Static
	static Texture *FindTexture (QString name);

private:
	// Internal
	void Reset			(void);
	bool Load			(QString filename);

private:
	// Variables
	QString mName;		// Texture name
	quint8 *mData;		// Image data

	quint32 mWidth;		// Texture width
	quint32 mHeight;	// Texture height
	quint32 mDepth;		// Pixel depth
	
	bool mLoaded;		// Is the texture loaded
	quint32 mTexID;		// OpenGL texture ID

	QRectF mMask;		// Texture mask

public:
	static QList<Texture *> Textures;
};

#endif // TEXTURE_H
