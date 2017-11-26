#ifndef __mass_H__MDBMA__
#define __mass_H__MDBMA__
#include "defs.hxx"
#include "obj3d.hxx"
#include "vec3d.hxx"


class frame;
class spring;
class room;

	// tablica 18tu springow (wskaznikow)
// Y |			 6   2  4  Z |		12   8 10	 Y |		16   2 14
//	 |			 1 x--  0	 |		 1 x--  0	   |		 9 x--  8
//	 |____X		 5   3  7	 |____X 11   9 13	   |_____Z  15   3 17

static double tmp2 = 0;


class mass : public object	/* pojedynczy punkt masy */
{
protected:
	friend class scene;
	friend class frame;
	friend class spring;

	spring*		array_springs[18];
	spring*		frame_spring;
	int			object_id;
   
	vector3 current_pos;      //wyliczona pozycja
	double* own_mass;            //masa
	double* own_k;				//opornosc osrodka
	double* own_fade_out;

	void run_physics_step();
	void run_physics_step_with_room(room* room);

	void create_obj(void);
public:
	mass(int ID = 0, const vector3& pos = vector3(0), const vector3& vel = vector3(0), double* dmass = NULL, double* dDelta = &tmp2, double* dK = NULL, double* dFadeOut = NULL);
	~mass(void);
	spring* operator[](int index);
	void opengl_draw(void);
};

#endif

