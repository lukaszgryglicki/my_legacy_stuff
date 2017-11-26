#ifndef __frame_H__MDBMA__
#define __frame_H__MDBMA__
#include "defs.hxx"
#include "obj3d.hxx"
#include "spring.hxx"

class frame : public object		/* ramka trzymajaca zelka */
{
protected:
	friend class scene;
	friend class spring;

	mass		mass_position[8];
	spring*		array_springs[8];
	int			object_id;

	void create_obj(void);
public:
	frame(int ID = 0);
	~frame(void);
	const mass* get_oposite_mass(int x, int y, int z) const;
	vector3* get_pos(int x, int y, int z) const;
	vector3* inc_pos(real x, real y, real z);
	vector3* inc_pos(vector3* pos);
	spring* operator[](int index);
	void opengl_draw(void);
};

#endif

