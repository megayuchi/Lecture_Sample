#pragma once

#include <math.h>
#include <windows.h>
#include <intrin.h>


struct MATRIX4
{
	union
	{
		struct
		{
			float	_11;
			float	_12;
			float	_13;
			float	_14;

			float	_21;
			float	_22;
			float	_23;
			float	_24;

			float	_31;
			float	_32;
			float	_33;
			float	_34;

			float	_41;
			float	_42;
			float	_43;
			float	_44;
		};
		struct
		{
			float f[4][4];
		};
	};
};
struct VECTOR4
{
	float x;
	float y;
	float z;
	float w;

	inline		void Set(float in_x, float in_y, float in_z, float in_w);

	inline	VECTOR4		operator +(const VECTOR4 &v) const;
	inline	VECTOR4		operator -(const VECTOR4 &v) const;
	inline	VECTOR4		operator *(const float	&f) const;
	inline	VECTOR4		operator *(const VECTOR4& v) const;
	inline	VECTOR4		operator /(const float	&f) const;
	inline	BOOL		operator==(const VECTOR4& v) const;


};

inline VECTOR4 VECTOR4::operator +(const VECTOR4 &v4) const
{
	VECTOR4	result;
	result.x = this->x + v4.x;
	result.y = this->y + v4.y;
	result.z = this->z + v4.z;
	result.w = this->w + v4.w;
	return	result;
}

inline VECTOR4 VECTOR4::operator -(const VECTOR4 &v4) const
{
	VECTOR4		result;
	result.x = this->x - v4.x;
	result.y = this->y - v4.y;
	result.z = this->z - v4.z;
	result.w = this->w - v4.w;
	return	result;
}

inline VECTOR4 VECTOR4::operator *(const float	&f) const
{
	VECTOR4		r;
	r.x = this->x * f;
	r.y = this->y * f;
	r.z = this->z * f;
	r.w = this->w * f;
	return	r;
}

inline VECTOR4 VECTOR4::operator *(const VECTOR4& v) const
{
	VECTOR4		r;
	r.x = this->x * v.x;
	r.y = this->y * v.y;
	r.z = this->z * v.z;
	r.w = this->w * v.w;
	return	r;
}

inline VECTOR4 VECTOR4::operator /(const float	&f) const
{
	VECTOR4		r;
	r.x = this->x / f;
	r.y = this->y / f;
	r.z = this->z / f;
	r.w = this->w / f;
	return	r;
}

inline	BOOL VECTOR4::operator==(const VECTOR4& v) const
{
	BOOL	bResult;
	if (this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w)
		bResult = TRUE;
	else
		bResult = FALSE;

	return	bResult;
}


inline void VECTOR4::Set(float in_x, float in_y, float in_z, float in_w)
{
	x = in_x;
	y = in_y;
	z = in_z;
	w = in_w;

}