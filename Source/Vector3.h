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

#ifndef VECTOR3_H
#define VECTOR3_H



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Defines a 3-component vector. </summary>

class Vector3
{
public:
	// Constructors
	Vector3 (void);
	Vector3 (float value);
	Vector3 (float x, float y, float z);

	// Methods
	float Length		(void) const;
	Vector3 Normalize	(void);

	float Dot			(const Vector3 &value) const;
	float Distance		(const Vector3 &value) const;
		
	// Operators
	Vector3 operator +=	(const Vector3 &value);
	Vector3 operator -=	(const Vector3 &value);
	Vector3 operator *=	(float value);
	Vector3 operator /=	(float value);

	Vector3 operator +	(const Vector3 &value) const;
	Vector3 operator -	(const Vector3 &value) const;
	Vector3 operator *	(const Vector3 &value) const;
	Vector3 operator *	(float value) const;
	Vector3 operator /	(float value) const;

	Vector3 operator -	(void) const;

	bool operator ==	(const Vector3 &value) const;
	bool operator !=	(const Vector3 &value) const;

public:
	// Variables
	float X, Y, Z;
};

#endif // VECTOR3_H
