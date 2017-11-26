#ifndef __VECTOR_H__MDBMA__
#define __VECTOR_H__MDBMA__
#include "vec3d.hxx"

struct vector4	/* wektor 4d */
{

	vector4(vector3 & u,double w = 1.0f);
	vector4(double x = 0.0f,double y = 0.0f,double z = 0.0f,double w = 1.0f);
	~vector4(){}

	double & operator[](int);
	const double & operator[](int) const;
	double * v();	
	vector4& clear();
	vector4& operator+= (const double);
	vector4& operator-= (const double);
	vector4& operator*= (const double);
	vector4& operator/= (const double);

	vector4& operator+= (const vector4&);
	vector4& operator-= (const vector4&);
	vector4  operator-() const;

	double magnitude() const;
	vector4& normalize();
	vector3 uniformized();
	operator vector3() {
		return uniformized();
	}
	double x,y,z,w;
};


vector4 operator+ (const vector4 & u, const vector4 & v);
vector4 operator- (const vector4 & u, const vector4 & v);

vector4 operator+ (const vector4 & v,const double a);
vector4 operator+ (const double a,const vector4 & v);
vector4 operator- (const vector4 & v,const double a);
vector4 operator- (const double a,const vector4 & v);
vector4 operator* (const vector4 & v,const double a);
vector4 operator* (const double a,const vector4 & v);
vector4 operator/ (const vector4 & v,const double a);
vector4 operator/ (const double a,const vector4 & v);



inline vector4::vector4(double x,double y,double z,double w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


inline vector4::vector4(vector3 & u,double w) {
	this->x = u.x;
	this->y = u.y;
	this->z = u.z;
	this->w = w;
}


inline double & vector4::operator [](int pos) {
	double * p = reinterpret_cast<double*>(this);
	return *(p+pos);
}


inline const double & vector4::operator [](int pos) const {
	const double * p = reinterpret_cast<const double*>(this);
	return *(p+pos);
}


inline double* vector4::v() {
	return reinterpret_cast<double*>(this);
}

inline vector4& vector4::clear() {
	x = y = z = w = 0.0f;
	return *this;
}

inline vector4& vector4::operator+=(const double a) {
	x += a;
	y += a;
	z += a;
	w += a;
	return *this;
}


inline vector4& vector4::operator-=(const double a) {
	x -= a;
	y -= a;
	z -= a;
	w -= a;
	return *this;
}


inline vector4& vector4::operator+=(const vector4 & v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline vector4& vector4::operator-=(const vector4 & v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}


inline vector4 vector4::operator-() const {
	return vector4(-x, -y, -z, -w);
}

inline vector4& vector4::normalize() {
	double m = magnitude();
	x /= m;
	y /= m;
	z /= m;
	w /= m;
	return *this;
}

inline vector3 vector4::uniformized() {
	return vector3(x/w, y/w, z/w);
}


inline double vector4::magnitude() const{
	return (double)sqrt( x*x + y*y + z*z + w*w);
}


inline vector4 operator+ (const vector4 & u, const vector4 & v) {
	vector4 tmp(u);
	tmp += v;
	return tmp;
}


inline vector4 operator- (const vector4 & u, const vector4 & v) {
	vector4 tmp(u);
	tmp -= v;
	return tmp;
}


inline vector4 operator+ (const vector4 & v, const double a) {
	vector4 tmp(v);
	tmp += a;
	return tmp;
}

inline vector4 operator+ (const double a, const vector4 & v) {
	vector4 tmp(v);
	tmp += a;
	return tmp;
}

inline vector4 operator- (const vector4 & v, const double a) {
	vector4 tmp(v);
	tmp -= a;
	return tmp;
}

inline vector4 operator- (const double a, const vector4 & v) {
	vector4 tmp(-v);
	tmp += a;
	return tmp;
}


inline vector4 operator* (const vector4 & v, const double a) {
	vector4 tmp(v);
	tmp *= a;
	return tmp;
}


inline vector4 operator* (const double a, const vector4 & v) {
	vector4 tmp(v);
	tmp *= a;
	return tmp;
}


inline vector4 operator/ (const vector4 & v, const double a) {
	vector4 tmp(v);
	tmp /= a;
	return tmp;
}


inline vector4 operator/ (const double a,const vector4 & v) {
	vector4 tmp;
	tmp[0] = a / v[0];
	tmp[1] = a / v[1];
	tmp[2] = a / v[2];
	tmp[3] = a / v[3];
	return tmp;
}
#endif
