/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Types.h"

namespace Ahmad
{
namespace Core
{

class IMatrix;

//! This vector is seldom to use, but maybe i need this one day
template <class T>
class IVector4
{
public:
	T x, y, z, w;

	IVector4() 
	{ x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
	
	IVector4( T _x, T _y, T _z )
	{ x = _x; y = _y; z = _z; w = 1.0f; }

	inline void Set( T _x, T _y, T _z, T _w = 1.0f )
	{ x = _x; y = _y; z = _z; w = _w; }

	inline T	GetLength( void ) 
	{ 
		return (T) sqrt( x*x + y*y + z*z ); 
	}

	inline T	GetSqrLength( void )
	{ 
		return ( x*x + y*y + z*z); 
	}

	inline void Cross( const IVector4<T> &v1, const IVector4<T> &v2 )
	{ 
		x = v1.y*v2.z - v1.z*v2.y;
		y = v1.z*v2.x - v1.x*v2.z;
		z = v1.x*v2.y - v1.y*v2.x; 
	}

	inline void Normalize( void )
	{
		T f = (T)sqrt(x*x + y*y + z*z);
		x /= f; y /= f; z /= f;
	}

	inline T	AngleWith( IVector4<T> &v )
	{ 
		return (T) acos( ((*this)*v) / (this->GetLength()*v.GetLength()) ); 
	}

	inline void Sub( const IVector4<T> &v1, const IVector4<T> &v2 )
	{
		x = v2.x - v1.x;
		y = v2.y - v1.y;
		z = v2.z - v1.z;
		w = 1.0f;
	}

	void operator +=( const IVector4<T> &v ) 
	{ 
		x += v.x; y += v.y; z += v.z; w += v.w; 
	}

	void operator -=( const IVector4<T> &v ) 
	{ 
		x -= v.x; y -= v.y; z -= v.z; w -= v.w; 
	}

	void operator *=( const IVector4<T> &v ) 
	{ 
		x *= v.x; y *= v.y; z *= v.z; 
	}

	void operator *=( const IMatrix &mat ) const
	{
		T _x, _y, _z, _w;
		
		_x = x*mat._11 + y*mat._21 + z*mat._31 + mat._41;
		_y = x*mat._12 + y*mat._22 + z*mat._32 + mat._42;
		_z = x*mat._13 + y*mat._23 + z*mat._33 + mat._43;
		_w = x*mat._14 + y*mat._24 + z*mat._34 + mat._44;

		x = _x/_w;
		y = _y/_w;
		z = _z/_w;
		w = 1.0f;
	}

	void operator /=( const IVector4<T> &v ) 
	{ 
		x /= v.x; y /= v.y; z /= v.z; 
	}

	IVector4<T> operator +( const IVector4<T> &v ) const 
	{ 
		return IVector( x+v.x, y+v.y, z+v.z ); 
	}

	IVector4<T> operator -( const IVector4<T> &v ) const 
	{ 
		return IVector( x-v.x, y-v.y, z-v.z ); 
	}

	IVector4<T> operator *( T f ) const
	{ 
		return IVector ( x*f, y*f, z*f ); 
	}

	IVector4<T> operator *( const IMatrix &mat ) const
	{
		IVector4<T> vec;
		
		vec.x = x*mat._11 + y*mat._21 + z*mat._31 + mat._41;
		vec.y = x*mat._12 + y*mat._22 + z*mat._32 + mat._42;
		vec.z = x*mat._13 + y*mat._23 + z*mat._33 + mat._43;
		vec.w = x*mat._14 + y*mat._24 + z*mat._34 + mat._44;

		vec.x = vec.x/vec.w;
		vec.y = vec.y/vec.w;
		vec.z = vec.z/vec.w;
		vec.w = 1.0f;

		return vec;
	}

	inline  T	operator *( const IVector4<T> &v ) const
	{ 
		return ( x*v.x + y*v.y + z*v.z ); 
	}
		
};

//! A not very complete Vector class
template <class T>
class IVector3
{
public:
	T x, y, z;

	IVector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	IVector3( T _x, T _y, T _z ) { x = _x; y = _y; z = _z; }

	inline void Set( T _x, T _y, T _z ) { x = _x; y = _y; z = _z; }

	inline T	GetLength( void ) { return (T) sqrt( x*x + y*y + z*z ); }

	inline T	GetSqrLength( void ) { return ( x*x + y*y + z*z); }

	inline void Cross( const IVector3 &v1, const IVector3 &v2 ) 
	{
		x = v1.y*v2.z - v1.z*v2.y;
		y = v1.z*v2.x - v1.x*v2.z;
		z = v1.x*v2.y - v1.y*v2.x;
	}

	inline void Normalize( void )
	{
		// Bugs fixed: it used to be = T f = (T)(x*x + y*y + z*z); see: 
		// no sqrt caused buildViewMatrix get the wrong result
		T f = (T)sqrt(x*x + y*y + z*z);
		x /= f; y /= f; z /= f;
	}

	inline T	AngleWith( IVector3 &v )
	{
		return (T) acos( ((*this)*v) / (this->GetLength()*v.GetLength()) );
	}

	inline void Sub( const IVector3 &v1, const IVector3 &v2 )
	{
		x = v2.x - v1.x;
		y = v2.y - v1.y;
		z = v2.z - v1.z;
	}

	void operator +=( const IVector3 &v ) 
	{
		x += v.x; y += v.y; z += v.z;
	}

	void operator -=( const IVector3 &v )
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	void operator *=( const IVector3 &v )
	{
		x *= v.x; y *= v.y; z *= v.z;
	}

	void operator *=( const IMatrix &mat ) const
	{
		T _x, _y, _z;
		
		_x = x*mat._11 + y*mat._21 + z*mat._31 + mat._41;
		_y = x*mat._12 + y*mat._22 + z*mat._32 + mat._42;
		_z = x*mat._13 + y*mat._23 + z*mat._33 + mat._43;

		x = _x; y = _y; z = _z;
	}

	void operator /=( const IVector3 &v )
	{
		x /= v.x; y /= v.y; z /= v.z;
	}

	IVector3 operator +( const IVector3 &v ) const
	{
		return IVector3<T>( x+v.x, y+v.y, z+v.z );
	}

	IVector3 operator -( const IVector3 &v ) const
	{
		return IVector3<T>( x-v.x, y-v.y, z-v.z );
	}

	IVector3 operator *( T f ) const
	{
		return IVector3<T> ( x*f, y*f, z*f );
	}

	IVector3 operator *( const IMatrix &mat ) const
	{
		IVector3<T> vec;
		T w;
		
		vec.x = x*mat._11 + y*mat._21 + z*mat._31 + mat._41;
		vec.y = x*mat._12 + y*mat._22 + z*mat._32 + mat._42;
		vec.z = x*mat._13 + y*mat._23 + z*mat._33 + mat._43;
		w	  = (T)(x*mat._14 + y*mat._24 + z*mat._34 + mat._44);
		
		vec.x = vec.x/w;
		vec.y = vec.y/w;
		vec.z = vec.z/w;

		MessageBox(0, "ass", "", 0);
		return vec;
	}

	inline  T	operator *( const IVector3 &v ) const
	{
		return (x*v.x + y*v.y + z*v.z );
	}
		
};	

template <class T>
class IVector2
{
public:
	T x, y;

	IVector2() { x = 0.0f; y = 0.0f; }
	IVector2( T _x, T _y ) { x = _x; y = _y; }

	inline void Set( T _x, T _y ) { x = _x; y = _y; }

	inline T	GetLength( void ) { return (T) sqrt( x*x + y*y ); }

	inline T	GetSqrLength( void ) { return ( x*x + y*y ); }

	inline void Cross( const IVector2 &v1, const IVector2 &v2 )
	{
		x = v1.y*v2.z - v1.z*v2.y;
		y = v1.z*v2.x - v1.x*v2.z;
	}

	inline void Normalize( void )
	{
		T f = (T)sqrt(x*x + y*y );
		x /= f; y /= f;
	}

	inline T	AngleWith( IVector2 &v )
	{
		return (T) acos( ((*this)*v) / (this->GetLength()*v.GetLength()) );
	}

	inline void Sub( const IVector2 &v1, const IVector2 &v2 )
	{
		x = v2.x - v1.x;
		y = v2.y - v1.y;
	}

	void operator +=( const IVector2 &v ) 
	{
		x += v.x; y += v.y;
	}

	void operator -=( const IVector2 &v )
	{
		x -= v.x; y -= v.y;
	}

	void operator *=( const IVector2 &v )
	{
		x *= v.x; y *= v.y;
	}

	void operator /=( const IVector2 &v )
	{
		x /= v.x; y /= v.y;
	}

	IVector2 operator +( const IVector2 &v ) const
	{
		return IVector2<T>( x+v.x, y+v.y );
	}

	IVector2 operator -( const IVector2 &v ) const
	{
		return IVector2<T>( x-v.x, y-v.y );
	}

	IVector2 operator *( T f ) const
	{
		return IVector2<T> ( x*f, y*f );
	}

	inline  T	operator *( const IVector2 &v ) const
	{
		return (x*v.x + y*v.y );
	}
};

typedef IVector4<i32> IVector4i;
typedef IVector4<f32> IVector4f;
typedef IVector3<i32> IVector3i;
typedef IVector3<f32> IVector3f;
typedef IVector2<i32> IVector2i;
typedef IVector2<f32> IVector2f;

}	// namespace Core
}	// namespace Ahmad

#endif	// _VECTOR_H_