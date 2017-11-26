#ifndef __OBJECT_H__MDBMA__
#define __OBJECT_H__MDBMA__
#include "defs.hxx"
#include "vec3d.hxx"

class scene;

static double tmp = 0;

class object		/* klasa bazowa, obiekt 3d */
{
protected:
	friend class scene;

	int			object_type;
	int			object_id;
	bool		object_selected;
	vector3	object_pos;
	vector3	object_old_pos;
	vector3	object_vel;
	double*		delta;

	virtual void set_object_id(int id) { object_id = id; };
	virtual void object_select(bool s) { object_selected = s; };
public:
	virtual vector3* get_pos(void) { return &object_pos; };
	virtual void set_pos(real x, real y, real z)
	{ object_old_pos = object_pos; object_pos.x = x; object_pos.y = y; object_pos.z = z; };
	virtual void set_pos(vector3* pos)
	{ object_old_pos = object_pos; object_pos = *pos; };
	virtual vector3* inc_pos(real x, real y, real z)
	{ object_old_pos = object_pos; object_pos.x += x; object_pos.y += y; object_pos.z += z; return &object_pos; };
	virtual vector3* inc_pos(vector3* pos)
	{ object_old_pos = object_pos; object_pos += *pos; return &object_pos; };
	object(int ID = 0, const vector3& pos = vector3(0), const vector3& vel = vector3(0), double* fDelta = &tmp)
		: object_pos(pos), object_old_pos(pos), object_vel(vel), object_id(ID), object_type(OBJ_ID_NONE), object_selected(FALSE), delta(fDelta)
	{ object_old_pos = object_pos - (*delta) * object_vel; };
	~object(void) {};
	int get_object_type(void) { return object_type; };
	const int get_object_id(void) const { return object_id; };
	const bool is_object_selected(void) const { return object_selected; };
};

#endif
