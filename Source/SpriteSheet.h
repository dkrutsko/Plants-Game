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

#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <QHash>
#include <QRect>
#include <QString>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class QSize;
class Texture;

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class SpriteSheet
{
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> </summary>

	class Sprite
	{
	public:
		// Constructors
		////////////////////////////////////////////////////////////////////////////////
		/// <summary> </summary>

		Sprite (void)
		{
			x = 0;
			y = 0;
			w = 0;
			h = 0;
			i = 0;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// <summary> </summary>

		Sprite (quint32 x, quint32 y, quint32 w, quint32 h)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->i = 0;
		}

	public:
		// Variables
		quint32 x;	// X position
		quint32 y;	// Y position
		quint32 w;	// Width
		quint32 h;	// Height
		quint32 i;	// Display list index
	};

public:
	// Constructors
	 SpriteSheet		(QString filename);
	~SpriteSheet		(void);

	// Draw
	void Draw			(QString name, qint32 x, qint32 y,
							qreal sx = 1, qreal sy = 1) const;

	// Accessors
	QString Name		(void) const;

	QSize Size			(QString name) const;
	quint32 Width		(QString name) const;
	quint32 Height		(QString name) const;
	
	bool Loaded			(void) const;

	const Texture *Tex	(void) const;

private:
	// Internal
	void Reset			(void);
	bool Load			(QString filename);

private:
	// Variables
	Texture *mTexture;	// Texture sheet
	bool mLoaded;		// Is the SpriteSheet loaded
	quint32 mList;		// Display list

	QHash<QString, Sprite> mData;
};

#endif // SPRITE_SHEET_H
