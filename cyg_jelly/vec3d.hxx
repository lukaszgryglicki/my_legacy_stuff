#ifndef __VECTOR3_H
#define __VECTOR3_H

#include <math.h>

struct vector3	/* wektor 3d */
{
	vector3(double v = 0.0f);
	vector3(double x,double y,double z);
	~vector3(){}

	double &			operator[](int);
	const double &	operator[](int) const;
	double *			v();
	const double *	v() const;
	vector3&		clear();

	vector3&	operator+= (double);
	vector3&	operator-= (double);
	vector3&	operator*= (double);
	vector3&	operator/= (double);

	vector3&	operator+= (const vector3&);
	vector3&	operator-= (const vector3&);
	
	vector3	operator-() const;
	vector3&	normalize();
	vector3	normalized() const;
	double		magnitude() const;
	double		magnitude2() const;
	const double		distance_from_point(const vector3& pt) const;
	double x,y,z;
};

const vector3 operator+ (const vector3 &u,const  vector3 &v);
const vector3 operator- (const vector3 &u,const  vector3 &v);

const vector3 operator+ (const vector3 & v,double a);
const vector3 operator+ (double a,const vector3 & v);
const vector3 operator- (const vector3 & v,double a);
const vector3 operator- (double a,const vector3 & v);
const vector3 operator* (const vector3 & v,double a);
const vector3 operator* (double a,const vector3 & v);
const vector3 operator/ (const vector3 & v,double a);
const vector3 operator/ (double a,const vector3 & v);

double dot(const vector3&u, const vector3 &v);
vector3 cross(const vector3 &u,const vector3 &v);

bool operator==(const vector3 & u,const vector3 & v);
bool operator!=(const vector3 & u,const vector3 & v);


inline vector3::vector3(double x,double y,double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}


inline vector3::vector3(double val) {
	x = val;
	y = val;
	z = val;
}


inline double & vector3::operator [](int pos) {
	double * p = reinterpret_cast<double*>(this);
	return *(p+pos);
}


inline const double & vector3::operator [](int pos) const {
	const double * p = reinterpret_cast<const double*>(this);
	return *(p+pos);
}


inline double* vector3::v() {
	return reinterpret_cast<double*>(this);
}


inline const double* vector3::v() const{
	return reinterpret_cast<const double*>(this);
}


inline vector3& vector3::clear() {
	x = y = z = 0.0f;
	return *this;
}


inline vector3& vector3::operator+=(double a) {
	x += a;
	y += a;
	z += a;
	return *this;
}


inline vector3& vector3::operator-=(double a) {
	x -= a;
	y -= a;
	z -= a;
	return *this;
}


inline vector3& vector3::operator*=(double a) {
	x *= a;
	y *= a;
	z *= a;
	return *this;
}


inline vector3& vector3::operator/=(double a) {
	x /= a;
	y /= a;
	z /= a;
	return *this;
}


inline vector3& vector3::operator+=(const vector3 &v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}


inline vector3& vector3::operator-=(const vector3 &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}


inline vector3 vector3::operator-() const {
	return vector3(-x, -y, -z);
}


inline vector3& vector3::normalize() {
	double m = magnitude();
	x /= m;
	y /= m;
	z /= m;
	return *this;
}

inline vector3 vector3::normalized() const {
	double m = magnitude();
	return vector3(x/m,y/m,z/m);
}

inline double vector3::magnitude() const{
	return (double)sqrt( x*x + y*y + z*z );
}


inline double vector3::magnitude2() const{
	return ( x*x + y*y + z*z );
}


inline const double vector3::distance_from_point(const vector3& pt) const{
	vector3 tmp = (*this) - pt;
	return tmp.magnitude();
}


inline const vector3 operator+ (const vector3 &u,const  vector3 &v) {
	vector3 tmp(u);
	tmp += v;
	return tmp;
}


inline const vector3 operator- (const vector3 &u,const vector3 &v) {
	vector3 tmp(u);
	tmp -= v;
	return tmp;
}


inline const vector3 operator+ (const vector3 & v,double a) {
	vector3 tmp(v);
	tmp += a;
	return tmp;
}


inline const vector3 operator+ (double a,const vector3 & v) {
	vector3 tmp(v);
	tmp += a;
	return tmp;
}


inline const vector3 operator- (const vector3 & v, double a) {
	vector3 tmp(v);
	tmp -= a;
	return tmp;
}


inline const vector3 operator- (double a,const vector3 & v) {
	vector3 tmp(-v);
	tmp += a;
	return tmp;
}


inline const vector3 operator* (const vector3 & v,double a) {
	vector3 tmp(v);
	tmp *= a;
	return tmp;
}


inline const vector3 operator* (double a,vector3 & v) {
	vector3 tmp(v);
	tmp *= a;
	return tmp;
}


inline const vector3 operator/ (const vector3 & v,double a) {
	vector3 tmp(v);
	tmp /= a;
	return tmp;
}


inline const vector3 operator/ (double a,const vector3 & v) {
	vector3 tmp;
	tmp[0] = a / v[0];
	tmp[1] = a / v[1];
	tmp[2] = a / v[2];
	return tmp;
}


inline double dot(const vector3&u, const vector3 &v) {
	return u.x*v.x+u.y*v.y+u.z*v.z;
}


inline vector3 cross(const vector3 &u, const vector3 &v) {
	return vector3(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}


inline bool operator==(const vector3& u,const vector3& v) {
	return (memcmp(&u,&v,sizeof(vector3))==0);
}


inline bool operator!=(const vector3& u,const vector3& v) {
	return !(u==v);
}


#endif

