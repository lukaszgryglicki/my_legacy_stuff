#ifndef __MATRIX4_H__MDBMA__
#define __MATRIX4_H__MDBMA__
#include "vec4d.hxx"
#include <utility>

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

struct matrix4 /* macierz 4x4 */
{
	matrix4();
	~matrix4(){}

	vector4&		operator [] (const int);
	const vector4& operator [] (const int) const;
	double*			get_v();	
	const double*	get_v() const;	
	matrix4& clear();
	matrix4& load_I();

	matrix4& load_translation(const vector3 &);
	matrix4& load_scale(const vector3 &);
	matrix4& load_rotx(const double angleDEG);
	matrix4& load_roty(const double angleDEG);
	matrix4& load_rotz(const double angleDEG);

	const matrix4 transposed() const;
	matrix4 invert();

	void	 bern_poly(const double t);

	vector4 v[4];
};

matrix4 operator* (const matrix4 &, const matrix4 &);
vector4 operator* (const matrix4 &, const vector4 &);
vector4 operator* (const matrix4 &, const vector3 &);


inline matrix4::matrix4() 
{
 //load_I();
}


inline vector4 & matrix4::operator [](const int pos) {
	return v[pos];
}


inline const vector4 & matrix4::operator [](const int pos) const {
	return v[pos];
}


inline double * matrix4::get_v() {
	return reinterpret_cast<double*>(v);
}


inline const double * matrix4::get_v() const{
	return reinterpret_cast<const double*>(v);
}


inline matrix4 & matrix4::clear() {
	memset(v,0,sizeof(v));
	return (*this);
}


inline matrix4 & matrix4::load_I() {
	memset(v,0,sizeof(v));
	for(int i = 0; i<4; ++i)
		v[i][i] = 1.0f;
	return (*this);
}


inline matrix4 & matrix4::load_translation(const vector3 & u) {
	load_I();
	for(int i = 0;i < 3; ++i)
		v[3][i] = u[i];
	return (*this);
}


inline matrix4 & matrix4::load_scale(const vector3 & u) {
	clear();
	for(int i = 0;i < 3; ++i)
		v[i][i] = u[i];
	v[3][3]=1.0f;
	return (*this);
}


inline matrix4 & matrix4::load_rotx(const double angleDEG) {
	double angleRAD = (double)M_PI*angleDEG/180.0f;
	load_I();
	v[1][1] = (double)cos(angleRAD);		v[2][1] = (double)-sin(angleRAD);
	v[1][2] = (double)sin(angleRAD);		v[2][2] = (double) cos(angleRAD);
	return (*this);
}


inline matrix4 & matrix4::load_roty(const double angleDEG) {
	double angleRAD = (double)M_PI*angleDEG/180.0f;
	load_I();
	v[0][0] = (double) cos(angleRAD);	v[2][0] = (double) sin(angleRAD);
	v[0][2] = (double)-sin(angleRAD);	v[2][2] = (double) cos(angleRAD);
	return (*this);
}


inline matrix4 & matrix4::load_rotz(const double angleDEG) {
	double angleRAD = (double)M_PI*angleDEG/180.0f;
	load_I();
	v[0][0] = (double)cos(angleRAD);		v[1][0] = (double)-sin(angleRAD);
	v[0][1] = (double)sin(angleRAD);		v[1][1] = (double) cos(angleRAD);
	return (*this);
}


inline const matrix4 matrix4::transposed() const{
	double aux;
	matrix4 tmp(*this);
	for (int i=0; i<3; ++i) 
		for (int j=i+1; j<4; ++j) {
			aux = tmp[i][j];
			tmp[i][j] = tmp[j][i];
			tmp[j][i] = aux;
		}
		return tmp;
}


inline void matrix4::bern_poly(const double t) {
	clear();
	v[0][0] = 1.0f;
	for (int i = 1; i < 4; ++i) {
		for (int j=0; j < i; ++j)
			v[i][j] = (1-t) * v[i-1][j];
		for (int j=1; j <= i; ++j)
			v[i][j] += t * v[i-1][j-1];
	}
}


inline matrix4 operator* (const matrix4 & u, const matrix4 & v) {
	matrix4 tmp;
	for(int i=0; i<4; ++i)
		for(int j=0; j<4; ++j) {
			tmp[i][j] = 0.0f;
			for(int k = 0; k<4; ++k) {
				tmp[i][j] += u[k][j] * v[i][k];
			}
		}
		return tmp;
}


inline vector4 operator* (const matrix4 & u, const vector4 & v) {
	vector4 tmp;
	tmp.clear();
	for(int j=0; j<4; ++j)
		for(int k=0; k<4; ++k)
			tmp[j]+= u[k][j] * v[k];
	return tmp;
}


inline vector4 operator* (const matrix4 & u,const vector3 & v) {
	vector4 tmp;

	for(int j=0; j<4; ++j) {
		tmp[j] = u[3][j];
		for(int k=0; k<3; ++k)
			tmp[j]+= u[k][j] * v[k];

	}
	return tmp;

}

#endif
