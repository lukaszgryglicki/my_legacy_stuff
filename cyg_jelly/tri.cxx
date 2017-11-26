#include "defs.hxx"
#include "tri.hxx"

tri::tri(void)
{ 
	vertices[0] = vertices[1] = vertices[2] = NULL; 
};

tri::tri(vector3* first, vector3* second, vector3* third) 
{			 
	vertices[0] = first; vertices[1] = second; vertices[2] = third;
};

tri::~tri(void)
{
};

void tri::set_vertices(vector3* first, vector3* second, vector3* third)
{ 
	vertices[0] = first; vertices[1] = second; vertices[2] = third; 
};

vector3* tri::get_vertex(int index) 
{ 
	return vertices[index]; 
};

void tri::set_vertex(vector3* v, int index) 
{ 
	//printf("debug setting vertex: %p (%f,%f,%f)\n", (void*)v, v->x, v->y, v->z);
	vertices[index] = v; 
};

void tri::opengl_draw()
{
	vector3 n;
	n = cross((*vertices[0])-(*vertices[1]), (*vertices[0])-(*vertices[2]));
	glNormal3dv(n.v());
	glVertex3dv(vertices[0]->v()); glVertex3dv(vertices[1]->v()); glVertex3dv(vertices[2]->v());
	// blended
	n.x = -n.x; n.y = -n.y; n.z = -n.z;
	glNormal3dv(n.v());
	glVertex3dv(vertices[2]->v()); glVertex3dv(vertices[1]->v()); glVertex3dv(vertices[0]->v());
};


