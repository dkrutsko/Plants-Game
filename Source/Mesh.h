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

#ifndef MESH_H
#define MESH_H

#include <QList>
#include <QString>

#include "Vector3.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Texture;

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single color channel. </summary>

class Channel
{
public:
	// Constructors
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Channel (void)
	{
		Color[0] = 0.0f;
		Color[1] = 0.0f;
		Color[2] = 0.0f;
		Color[3] = 1.0f;

		Tex = NULL;
	}

public:
	// Variables
	float Color[4];	// Color
	Texture *Tex;	// Texture
};

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single material. </summary>

class Material
{
public:
	// Constructors
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Material (QString name = "")
	{
		Name = name;

		Diffuse.Color[0] = 1.0f;
		Diffuse.Color[1] = 1.0f;
		Diffuse.Color[2] = 1.0f;

		Alpha = 1.0f;
		Illum = 1.0f;
		Shininess = 2.0f;
	}

public:
	// Variables
	Channel Ambient;	// Ambience
	Channel Diffuse;	// Diffuse
	Channel Specular;	// Specular
	Channel Emissive;	// Emissive

	QString Name;		// Material Name

	float Alpha;		// Alpha
	float Illum;		// Illumination
	float Shininess;	// Shininess
};

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single face. </summary>

class Face
{
public:
	// Constructors
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Face (qint32 v1,  qint32 v2,  qint32 v3,
		  qint32 n1,  qint32 n2,  qint32 n3,
		  qint32 uv1, qint32 uv2, qint32 uv3, Vector3 *v)
	{
		Mat = NULL;

		Verts[0] = v1;  Verts[1] = v2;  Verts[2] = v3;
		Norms[0] = n1;  Norms[1] = n2;  Norms[2] = n3;
		UVs  [0] = uv1; UVs  [1] = uv2; UVs  [2] = uv3;

		Center = (v[v1] + v[v2] + v[v3]) / 3.0f;
		Normal = (v[v1] - v[v2]).Normalize() *
				 (v[v1] - v[v3]).Normalize();
	}

public:
	// Variables
	Material *Mat;		// Material

	int Verts[3];		// Vertices
	int Norms[3];		// Normals
	int UVs[3];			// UV Coords

	Vector3 Center;		// Face Center
	Vector3 Normal;		// Face Normal
};

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single Mesh. </summary>

class Mesh
{
public:
	// Constructors
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>

	Mesh (QString name = "")
	{
		Name		= name;

		Verts		= NULL;
		Norms		= NULL;
		UVs			= NULL;

		VertCount	= 0;

		StartFrame	= 0;
		StopFrame	= 0;
		CurrFrame	= 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Destructor deallocating class resources. </summary>

	virtual ~Mesh (void)
	{
		Name.clear();

		delete[] Verts;
		delete[] Norms;
		delete[] UVs;

		while (Faces.size() > 0)
			delete[] Faces.takeAt(0);

		VertCount	= 0;

		StartFrame	= 0;
		StopFrame	= 0;
		CurrFrame	= 0;

		Frames.clear();
	}

	// Utility
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Adds a face to this mesh object. </summary>

	void addFace (Face *face)
	{
		Faces << face;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Adds a face to this mesh object. </summary>
	
	void addFace (qint32 v1,  qint32 v2,  qint32 v3,
				  qint32 n1,  qint32 n2,  qint32 n3,
				  qint32 uv1, qint32 uv2, qint32 uv3, Material *mat)
	{
		Face *lFace = new Face (v1, v2, v3,
			n1, n2, n3, uv1, uv2, uv3, Verts);

		lFace->Mat = mat;
		Faces << lFace;
	}

public:
	// Variables
	QString Name;			// Material Name

	Vector3 *Verts;			// Vertices
	Vector3 *Norms;			// Normals
	Vector3 *UVs;			// UV Coords

	quint32 VertCount;		// Number of Vertices
	QList<Face *> Faces;	// Faces

	// Animation
	float StartFrame;		// Start Frame
	float StopFrame;		// Stop Frame
	float CurrFrame;		// Current Frame

	QList <qreal *> Frames;	// Frames
};

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a simple 3D object. </summary>

class Object
{
public:
	// Constructors
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Constructor instantiating this class. </summary>
		
	Object (QString name)
	{
		Mesh = NULL;
		Name = name;
	}

public:
	// Variables
	Mesh *Mesh;		// Mesh Data
	QString	Name;	// Object Name
	
	QList<Object *> Children;
};

#endif // MESH_H
