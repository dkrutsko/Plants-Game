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

#include <fbxsdk.h>

#include "Mesh.h"
#include "Model.h"
#include "Vector3.h"
#include "Texture.h"

#include <QDebug>
#include <QString>
#include <QFileInfo>



//----------------------------------------------------------------------------//
// Constructors                                                         Model //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Model::Model (QString pFilename, bool useDisplayList)
{
	mList    = 0;
	mUseList = useDisplayList;
	mLoaded  = Load (pFilename);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

Model::~Model (void)
{
	while (mObjects.size() > 0)
		delete mObjects.takeAt(0);

	while (mMaterials.size() > 0)
		delete mMaterials.takeAt(0);

	mList    = 0;
	mUseList = false;
	mLoaded  = false;
}



//----------------------------------------------------------------------------//
// Update & Draw                                                        Model //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::Update (quint32 pGametime)
{
	if (!mLoaded)	return;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::Draw (quint32 pGametime, const Vector3 &pPosition,
	const Vector3 &pRotation, const Vector3 &pScale)
{
	if (!mLoaded)	return;
	
	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f		(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode	(GL_MODELVIEW);
	glPushMatrix	();
	glLoadIdentity	();

	glTranslatef	(pPosition.X, pPosition.Y, pPosition.Z);
	glRotatef		(pRotation.X, 1, 0, 0);
	glRotatef		(pRotation.Y, 0, 1, 0);
	glRotatef		(pRotation.Z, 0, 0, 1);
	glScalef		(pScale.X, pScale.Y, pScale.Z);

	if (!mUseList)
		for (qint32 i = 0; i < mObjects.size(); ++i)
			DrawObject (mObjects.at (i), pPosition, pRotation, pScale);

	else if (mList == 0)
	{
		mList = glGenLists(1);
		glNewList (mList, GL_COMPILE_AND_EXECUTE);

		for (qint32 i = 0; i < mObjects.size(); ++i)
			DrawObject (mObjects.at (i), pPosition, pRotation, pScale);

		glEndList();
	}

	else glCallList	(mList);

	glMatrixMode	(GL_MODELVIEW);
	glPopMatrix		();
}



//----------------------------------------------------------------------------//
// Accessors                                                            Model //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Model::Loaded (void) const
{
	return mLoaded;
}



//----------------------------------------------------------------------------//
// Internal                                                             Model //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

bool Model::Load (QString pFilename)
{
	QByteArray lData = pFilename.toAscii();
	const char *lFilename = lData.constData();

	//----------------------------------------------------------------------------//
	// Initialize                                                                 //
	//----------------------------------------------------------------------------//

	KFbxSdkManager	*lSdkManager;
	KFbxScene		*lScene;

	lSdkManager = KFbxSdkManager::Create();
	if (!lSdkManager)
	{
		qWarning() << "Unable to create the FBX Manager:" << pFilename;
		return false;
	}

	KFbxIOSettings *lIos = KFbxIOSettings::Create (lSdkManager, IOSROOT);

	lSdkManager->SetIOSettings (lIos);
	lScene = KFbxScene::Create (lSdkManager, "");



	//----------------------------------------------------------------------------//
	// Import                                                                     //
	//----------------------------------------------------------------------------//

	KFbxImporter *lImporter = KFbxImporter::Create (lSdkManager, "");
	bool lImportStatus = lImporter->Initialize (lFilename, -1, lIos);

	if (!lImportStatus)
	{
		qWarning() << "Unable to initialize the importer"
				   << pFilename << ": " << lImporter->GetLastErrorString();

		if (lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_YET ||
			lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_ANYMORE)
		{
			qint32 lSDKMajor,  lSDKMinor,  lSDKRev,
				   lFileMajor, lFileMinor, lFileRev;

			KFbxSdkManager::GetFileFormatVersion (lSDKMajor,  lSDKMinor,  lSDKRev);
			lImporter->GetFileVersion			 (lFileMajor, lFileMinor, lFileRev);

			qWarning ("FBX version: %d.%d.%d, File version: %d.%d.%d",
				lSDKMajor, lSDKMinor, lSDKRev, lFileMajor, lFileMinor, lFileRev);
		}

		return false;
	}

	if (lImporter->IsFBX())
	{
		lIos->SetBoolProp (IMP_FBX_MATERIAL,		true);
		lIos->SetBoolProp (IMP_FBX_TEXTURE,			true);
		lIos->SetBoolProp (IMP_FBX_LINK,			false);
		lIos->SetBoolProp (IMP_FBX_SHAPE,			false);
		lIos->SetBoolProp (IMP_FBX_GOBO,			false);
		lIos->SetBoolProp (IMP_FBX_ANIMATION,		false);
		lIos->SetBoolProp (IMP_FBX_GLOBAL_SETTINGS,	true);
	}

	if (!lImporter->Import (lScene))
	{
		qWarning() << "Unable to import the scene" << pFilename
				   << ": " << lImporter->GetLastErrorString();
		return false;
	}

	lImporter->Destroy();



	//----------------------------------------------------------------------------//
	// Load                                                                       //
	//----------------------------------------------------------------------------//

	KFbxNode *lNode = lScene->GetRootNode();
	QString   lPath = QFileInfo (pFilename).path() + "/";

	if (lNode)
	{
		LoadTextures (lScene, lPath);
		
		qint32 lChildCount = lNode->GetChildCount();
		for (qint32 i = 0; i < lChildCount; ++i)
			LoadContent (lNode->GetChild(i), &mObjects);
	}



	//----------------------------------------------------------------------------//
	// Finalize                                                                   //
	//----------------------------------------------------------------------------//

	if (lSdkManager) 
		lSdkManager->Destroy();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::Triangulate (KFbxNode *pNode)
{
	// Not used but kept in for reference
	// Converts and Triangulates Nurbs, Mesh, Patches and Surfaces
	KFbxNodeAttribute *lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute &&
	   (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH ||
		lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB ||
		lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURBS_SURFACE ||
		lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH))
	{
		KFbxGeometryConverter lConverter (pNode->GetFbxSdkManager());
		lConverter.TriangulateInPlace    (pNode);
	}

	qint32 lChildCount = pNode->GetChildCount();
	for (qint32 i = 0; i < lChildCount; ++i)
		Triangulate (pNode->GetChild (i));
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::LoadTextures (KFbxScene *pScene, QString pPath)
{
	qint32 lTextureCount = pScene->GetTextureCount();
	for (qint32 i = 0; i < lTextureCount; ++i)
	{
		KFbxFileTexture *lFileTexture = KFbxCast
			<KFbxFileTexture> (pScene->GetTexture(i));

		if (!lFileTexture || lFileTexture->GetUserDataPtr())
			continue;

		QFileInfo lFileInfo = lFileTexture->GetFileName();
		lFileInfo			= pPath + lFileInfo.fileName();

		// Only Targa textures are supported
		if (lFileInfo.suffix().toUpper() != "TGA")
		{
			qWarning() << "Only TGA textures are supported:"
					   << lFileInfo.absoluteFilePath();
			continue;
		}

		Texture *lTexture = Texture::FindTexture (lFileInfo.fileName());
		if (lTexture == NULL)
			lTexture = new Texture (lFileInfo.absoluteFilePath(), lFileInfo.fileName());

		if (!lTexture->Loaded())
		{
			qWarning() << "Unable to load texture file:"
					   << lFileInfo.absoluteFilePath();

			delete lTexture;
			continue;
		}

		lFileTexture->SetUserDataPtr (lTexture);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::LoadContent (KFbxNode *pNode, QList<Object *> *pObjects)
{
	QList<Object *> *lNextList = pObjects;

	if (pNode->GetNodeAttribute() != NULL &&
		pNode->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eMESH)
		LoadMesh (pNode, lNextList);

	qint32 lChildCount = pNode->GetChildCount();
	for (qint32 i = 0; i < lChildCount; ++i)
		LoadContent (pNode->GetChild(i), lNextList);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::LoadMesh (KFbxNode *pNode, QList<Object *> *pObjects)
{
	Object *lObject = new Object (pNode->GetName());

	//----------------------------------------------------------------------------//
	// Materials                                                                  //
	//----------------------------------------------------------------------------//
	
	Material *lMaterial = new Material();

	qint32 lMaterialCount = pNode->GetMaterialCount();
	for (qint32 i = 0; i < lMaterialCount; ++i)
	{
		KFbxSurfaceMaterial *lSurfaceMaterial = pNode->GetMaterial(i);
		if (lSurfaceMaterial && !lSurfaceMaterial->GetUserDataPtr() &&
			lSurfaceMaterial->Is (KFbxSurfaceLambert::ClassId))
		{
			lMaterial->Name = lSurfaceMaterial->GetName();

			// For our purposes we will only get the first valid material
			ApplyMaterialProperty (lSurfaceMaterial, KFbxSurfaceMaterial::sAmbient,
				KFbxSurfaceMaterial::sAmbientFactor, &lMaterial->Ambient);
			ApplyMaterialProperty (lSurfaceMaterial, KFbxSurfaceMaterial::sDiffuse,
				KFbxSurfaceMaterial::sDiffuseFactor, &lMaterial->Diffuse);
			ApplyMaterialProperty (lSurfaceMaterial, KFbxSurfaceMaterial::sSpecular,
				KFbxSurfaceMaterial::sSpecularFactor, &lMaterial->Specular);
			ApplyMaterialProperty (lSurfaceMaterial, KFbxSurfaceMaterial::sEmissive,
				KFbxSurfaceMaterial::sEmissiveFactor, &lMaterial->Emissive);

			KFbxProperty lShininessProperty = lSurfaceMaterial->
				FindProperty (KFbxSurfaceMaterial::sShininess);

			if (lShininessProperty.IsValid())
				lMaterial->Shininess = KFbxGet<float> (lShininessProperty);

			break;
		}
	}

	mMaterials << lMaterial;



	//----------------------------------------------------------------------------//
	// Mesh                                                                       //
	//----------------------------------------------------------------------------//

	KFbxMesh *lMesh = pNode->GetMesh();

	KFbxVector4 *lControlPoints = lMesh->GetControlPoints();
	KFbxLayerElementArrayTemplate<KFbxVector4>* lNorms = NULL;
	KFbxLayerElementArrayTemplate<KFbxVector2>* lUVs   = NULL;

	lMesh->GetNormals   (&lNorms);
	lMesh->GetTextureUV (&lUVs);

	lObject->Mesh = new Mesh();

	lObject->Mesh->Verts = new Vector3[lMesh->GetControlPointsCount()];
	lObject->Mesh->Norms = new Vector3[lMesh->GetControlPointsCount()];
	lObject->Mesh->UVs   = new Vector3[lMesh->GetTextureUVCount()    ];

	lObject->Mesh->VertCount = lMesh->GetControlPointsCount();

	for (quint32 i = 0; i < lObject->Mesh->VertCount; ++i)
	{
		lObject->Mesh->Verts[i] = Vector3 ((float) lControlPoints[i][0],
			(float) lControlPoints[i][1], (float) lControlPoints[i][2]);
		lObject->Mesh->Norms[i] = Vector3 ((float) lNorms->GetAt (i)[0],
			(float) lNorms->GetAt (i)[1], (float) lNorms->GetAt (i)[2]);
	}

	for (qint32 i = 0; i < lMesh->GetTextureUVCount(); ++i)
		lObject->Mesh->UVs[i] = Vector3 ((float) lUVs->GetAt(i)[0],
			(float) lUVs->GetAt(i)[1], (float) lUVs->GetAt(i)[2]);

	qint32 lPolygonCount = lMesh->GetPolygonCount();
	for (qint32 i = 0; i < lPolygonCount; ++i)
	{
		// Triangulation Code
		qint32 lPolygonSize = lMesh->GetPolygonSize(i);
		for (qint32 j = 1; j < lPolygonSize - 1; ++j)
		{
			qint32 lIndex1 = lMesh->GetPolygonVertex (i, 0);
			qint32 lIndex2 = lMesh->GetPolygonVertex (i, j);
			qint32 lIndex3 = lMesh->GetPolygonVertex (i, j+1);

			qint32 lUV1 = lMesh->GetTextureUVIndex (i, 0);
			qint32 lUV2 = lMesh->GetTextureUVIndex (i, j);
			qint32 lUV3 = lMesh->GetTextureUVIndex (i, j+1);

			lObject->Mesh->addFace (lIndex1, lIndex2, lIndex3,
				lIndex1, lIndex2, lIndex3, lUV1, lUV2, lUV3, lMaterial);
		}
	}



	//----------------------------------------------------------------------------//
	// Animation                                                                  //
	//----------------------------------------------------------------------------//
	
	// We will just get the global position for now
	qreal *lPosition = new double[16];

	memcpy (lPosition, (qreal *) lMesh->GetNode()->
		EvaluateGlobalTransform(), 16 * sizeof (qreal));

	lObject->Mesh->Frames << lPosition;

	*pObjects << lObject;
	 pObjects = &lObject->Children;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::ApplyMaterialProperty (const KFbxSurfaceMaterial *pSurfaceMaterial,
	const char *pPropertyName, const char *pFactorName, Channel *pChannel)
{
	fbxDouble3 lResult (0, 0, 0);
	double     lFactor = 0;

	KFbxProperty lProperty       = pSurfaceMaterial->FindProperty (pPropertyName);
	KFbxProperty lFactorProperty = pSurfaceMaterial->FindProperty (pFactorName);

	if (!lProperty.IsValid()) return;

	if (lFactorProperty.IsValid())
	{
		lResult = KFbxGet<fbxDouble3> (lProperty);
		lFactor = KFbxGet<double>     (lFactorProperty);

		pChannel->Color[0] = lResult[0] * lFactor;
		pChannel->Color[1] = lResult[1] * lFactor;
		pChannel->Color[2] = lResult[2] * lFactor;
	}

	if (lProperty.GetSrcObjectCount (KFbxFileTexture::ClassId))
	{
		KFbxFileTexture* lTexture = lProperty.GetSrcObject (FBX_TYPE (KFbxFileTexture), 0);

		if (lTexture && lTexture->GetUserDataPtr())
			pChannel->Tex = (Texture *) lTexture->GetUserDataPtr();
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> </summary>

void Model::DrawObject (Object *pObject, const Vector3 &pPosition,
	const Vector3 &pRotation, const Vector3 &pScale)
{
	if (pObject == NULL)
		return;

	Mesh *lMesh = pObject->Mesh;
	Material *lLastMaterial = NULL;

	glPushMatrix	();
	glMultMatrixd	(lMesh->Frames.at (lMesh->CurrFrame));

	qint32 lFaceCount = lMesh->Faces.size();
	for (qint32 i = 0; i < lFaceCount; ++i)
	{
		Face *lFace = lMesh->Faces.at(i);
		Material *lMaterial = lFace->Mat;

		if (lMaterial != lLastMaterial)
		{
			lLastMaterial = lMaterial;
			lMaterial->Diffuse.Color[3] = lMaterial->Alpha;

			glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,  (GLfloat *) lMaterial->Ambient.Color);
			glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,  (GLfloat *) lMaterial->Diffuse.Color);
			glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *) lMaterial->Specular.Color);
			glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *) lMaterial->Emissive.Color);
			glMaterialf  (GL_FRONT_AND_BACK, GL_SHININESS, lMaterial->Shininess);

			if (lMaterial->Diffuse.Tex != NULL)
			{
				glEnable      (GL_TEXTURE_2D);
				glBindTexture (GL_TEXTURE_2D, lMaterial->Diffuse.Tex->TexID());
			}
			
			else glDisable (GL_TEXTURE_2D);
		}

		glBegin (GL_TRIANGLES);
		for (int v = 0; v < 3; ++v)
		{
			if (lFace->UVs[v] != -1)
				glTexCoord2f (lMesh->UVs [lFace->UVs[v]].X,
							  lMesh->UVs [lFace->UVs[v]].Y);

			glNormal3f (lMesh->Norms [lFace->Norms[v]].X,
						lMesh->Norms [lFace->Norms[v]].Y,
						lMesh->Norms [lFace->Norms[v]].Z);

			glVertex3f (lMesh->Verts [lFace->Verts[v]].X,
						lMesh->Verts [lFace->Verts[v]].Y,
						lMesh->Verts [lFace->Verts[v]].Z);
		}
		glEnd();
	}

	glPopMatrix  ();

	for (qint32 i = 0; i < pObject->Children.size(); ++i)
		DrawObject (pObject->Children.at (i), pPosition, pRotation, pScale);
}
