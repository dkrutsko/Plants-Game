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

#ifndef MODEL_H
#define MODEL_H

#include <QList>
#include <QString>

#include "Vector3.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

class Object;
class Material;
class Channel;

#include <fbxfilesdk/fbxfilesdk_nsbegin.h>

class KFbxNode;
class KFbxScene;
class KFbxSurfaceMaterial;

#include <fbxfilesdk/fbxfilesdk_nsend.h>
#include <fbxfilesdk/fbxfilesdk_nsuse.h>

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

class Model
{
public:
	// Constructors
	 Model				(QString filename, bool useDisplayList = true);
	~Model				(void);

	// Update & Draw
	void Update			(quint32 gametime);
	void Draw			(quint32 gametime,
						 const Vector3 &position = Vector3 (0, 0, 0),
						 const Vector3 &rotation = Vector3 (0, 0, 0),
						 const Vector3 &scale    = Vector3 (1, 1, 1));

	// Accessors
	bool Loaded			(void) const;

private:
	// Internal
	bool Load			(QString filename);

	void Triangulate	(KFbxNode  *node);
	void LoadTextures	(KFbxScene *scene, QString path);

	void LoadContent	(KFbxNode *node, QList<Object *> *objects);
	void LoadMesh		(KFbxNode *node, QList<Object *> *objects);

	void ApplyMaterialProperty (const KFbxSurfaceMaterial *surfaceMaterial,
		const char *propertyName, const char *factorName, Channel *channel);

	void DrawObject		(Object *object, const Vector3 &position,
						 const Vector3 &rotation, const Vector3 &scale);

private:
	// Variables
	bool mLoaded;	// Is the model loaded

	quint32 mList;	// Display list
	bool mUseList;	// Use display list

	QList<Object   *> mObjects;
	QList<Material *> mMaterials;
};

#endif // MODEL_H
