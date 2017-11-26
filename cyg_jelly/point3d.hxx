#ifndef __POINT_H__MDBMA__
#define __POINT_H__MDBMA__
#include "defs.hxx"
#include "obj3d.hxx"
#include "vec3d.hxx"

class point3 : public object, public vector3	/* punkt 3d */
{
	vector3	my_Normal;
	real		my_U;
	real		my_V;
public:
	point3(double v = 0.0f) : vector3(v) {};
	point3(double x,double y,double z) : vector3(x, y, z) {};
	~point3() {};

	vector3& get_normal(void) { return my_Normal; };
	real& get_u(void) { return my_U; };
	real& get_v(void) { return my_V; };
	void set_params(real u, real v) { my_U = u; my_V = v; };

	point3& operator =(const point3& pt)
	{
		my_U = pt.my_U; my_V = pt.my_V;
		my_Normal = pt.my_Normal;
		vector3::operator =(pt);
		return *this;
	};
};

#endif
