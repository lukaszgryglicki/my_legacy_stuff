#ifndef __TRI_H__MDBMA__
#define __TRI_H__MDBMA__
#include "defs.hxx"
#include "vec3d.hxx"
#include "obj3d.hxx"


class tri : public object	/* trojkat */
{
	vector3* vertices[3];
public:
	tri(void);
	tri(vector3* first, vector3* second, vector3* third) ;
	~tri(void);

	void set_vertices(vector3* first, vector3* second, vector3* third);
	vector3* get_vertex(int index);
	void set_vertex(vector3* v, int index);
	void opengl_draw();
};

#endif
