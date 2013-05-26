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

#include <cmath>
#include "Vector3.h"



//----------------------------------------------------------------------------//
// Constructors                                                       Vector3 //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>

Vector3::Vector3 (void)
{
	X = 0;
	Y = 0;
	Z = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>

Vector3::Vector3 (float pValue)
{
	X = pValue;
	Y = pValue;
	Z = pValue;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Constructor instantiating this class. </summary>

Vector3::Vector3 (float pX, float pY, float pZ)
{
	X = pX;
	Y = pY;
	Z = pZ;
}



//----------------------------------------------------------------------------//
// Methods                                                            Vector3 //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the length of this vector. </summary>

float Vector3::Length (void) const
{
	return sqrt (X * X + Y * Y + Z * Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Normalizes this vector. </summary>

Vector3 Vector3::Normalize (void)
{
	float lLength = Length();

	X /= lLength;
	Y /= lLength;
	Z /= lLength;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the dot product of two vectors. </summary>

float Vector3::Dot (const Vector3 &pValue) const
{
	return (X * pValue.X + Y * pValue.Y + Z * pValue.Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the distance between two vectors. </summary>

float Vector3::Distance (const Vector3 &pValue) const
{
	float lDisX = pValue.X - X;
	float lDisY = pValue.Y - Y;
	float lDisZ = pValue.Z - Z;

	return sqrt (lDisX * lDisX + lDisY * lDisY + lDisZ * lDisZ);
}



//----------------------------------------------------------------------------//
// Operators                                                          Vector3 //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Adds this vector to the specified vector. </summary>

Vector3 Vector3::operator += (const Vector3 &pValue)
{
	return (*this = (*this + pValue));
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Subtracts this vector to the specified vector. </summary>

Vector3 Vector3::operator -= (const Vector3 &pValue)
{
	return (*this = (*this - pValue));
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Multiplies this vector by a specified value. </summary>

Vector3 Vector3::operator *= (float pValue)
{
	return (*this = (*this * pValue));
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Divides this vector by a specified value. </summary>

Vector3 Vector3::operator /= (float pValue)
{
	return (*this = (*this / pValue));
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the sum of two vectors. </summary>

Vector3 Vector3::operator + (const Vector3 &pValue) const
{
	return Vector3 (pValue.X + X, pValue.Y + Y, pValue.Z + Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the difference of two vectors. </summary>

Vector3 Vector3::operator - (const Vector3 &pValue) const
{
	return Vector3 (pValue.X - X, pValue.Y - Y, pValue.Z - Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the cross product of two vectors. </summary>

Vector3 Vector3::operator * (const Vector3 &pValue) const
{
	return Vector3 (Y * pValue.Z - Z * pValue.Y,
					Z * pValue.X - X * pValue.Z,
					X * pValue.Y - Y * pValue.X);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the product of this vector and a value. </summary>
Vector3 Vector3::operator * (float pValue) const
{
	return Vector3 (X * pValue, Y * pValue, Z * pValue);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the quotient of this vector and a value. </summary>

Vector3 Vector3::operator / (float pValue) const
{
	return Vector3 (X / pValue, Y / pValue, Z / pValue);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns a vector pointing in the opposite direction. </summary>

Vector3 Vector3::operator - (void) const
{
	return Vector3 (-X, -Y, -Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns whether this vector is equal to another. </summary>

bool Vector3::operator == (const Vector3 &pValue) const
{
	return (pValue.X == X && pValue.Y == Y && pValue.Z == Z);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns whether this vector is not equal to another. </summary>

bool Vector3::operator != (const Vector3 &pValue) const
{
	return !(pValue == *this);
}
