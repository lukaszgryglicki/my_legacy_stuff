#ifndef __spring_H__MDBMA__
#define __spring_H__MDBMA__
#include "defs.hxx"
#include "obj3d.hxx"
#include "mass.hxx"

class spring			/* sprezynka */
{
protected:
	mass* first_mass;
	mass* second_mass;

	real default_length;
	real* my_fC;
public:
	spring(mass* first = NULL, mass* second = NULL, real defLength = 0, real* fC = NULL) { first_mass = first; second_mass = second; default_length = defLength; my_fC = fC; };
	~spring(void) {};

	mass* get_first_mass(void) { return first_mass; };
	mass* get_second_mass(void) { return second_mass; };
	mass* get_oposite_mass(mass* m) { if (m != first_mass && m != second_mass) return NULL; if(m == first_mass) return second_mass; else return first_mass; };
	real get_def_length(void) { return default_length; };
	real get_c(void) { return *my_fC; };
	void set_c(real fC) { *my_fC = fC; };
	void opengl_draw(void)
	{ 
		glVertex3d(first_mass->object_pos.x, first_mass->object_pos.y, first_mass->object_pos.z); 
		glVertex3d(second_mass->object_pos.x, second_mass->object_pos.y, second_mass->object_pos.z); 
	};
};

#endif

